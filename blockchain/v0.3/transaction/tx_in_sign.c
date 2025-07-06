#include <string.h>
#include "blockchain.h"

/**
 * find_matching_utxo - Find UTXO that matches the given hash
 * @all_unspent: List of all unspent transaction outputs
 * @tx_out_hash: Hash to match against
 *
 * Return: Pointer to matching UTXO, or NULL if not found
 */
static unspent_tx_out_t *find_matching_utxo(llist_t *all_unspent,
					     uint8_t *tx_out_hash)
{
	unspent_tx_out_t *utxo;
	int i, list_size;

	if (!all_unspent || !tx_out_hash)
		return (NULL);

	list_size = llist_size(all_unspent);
	for (i = 0; i < list_size; i++)
	{
		utxo = (unspent_tx_out_t *)llist_get_node_at(all_unspent, i);
		if (!utxo)
			continue;

		if (memcmp(utxo->out.hash, tx_out_hash,
			   SHA256_DIGEST_LENGTH) == 0)
			return (utxo);
	}

	return (NULL);
}

/**
 * validate_ownership - Check if sender owns the UTXO
 * @sender: Private key of the sender
 * @utxo: Unspent transaction output to validate
 *
 * Return: 1 if sender owns the UTXO, 0 otherwise
 */
static int validate_ownership(EC_KEY const *sender, unspent_tx_out_t *utxo)
{
	uint8_t sender_pub[EC_PUB_LEN];

	if (!sender || !utxo)
		return (0);

	if (!ec_to_pub(sender, sender_pub))
		return (0);

	if (memcmp(sender_pub, utxo->out.pub, EC_PUB_LEN) != 0)
		return (0);

	return (1);
}

/**
 * tx_in_sign - Signs a transaction input
 * @in: Points to the transaction input structure to sign
 * @tx_id: Contains the ID (hash) of the transaction
 * @sender: Contains the private key of the sender
 * @all_unspent: List of all unspent transaction outputs
 *
 * Return: Pointer to the resulting signature structure, or NULL on failure
 */
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
		  EC_KEY const *sender, llist_t *all_unspent)
{
	unspent_tx_out_t *utxo;

	/* Validate input parameters */
	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);

	/* Find the UTXO that matches the input's tx_out_hash */
	utxo = find_matching_utxo(all_unspent, in->tx_out_hash);
	if (!utxo)
		return (NULL);

	/* Validate that sender owns this UTXO */
	if (!validate_ownership(sender, utxo))
		return (NULL);

	/* Clear signature structure */
	memset(&in->sig, 0, sizeof(sig_t));

	/* Sign the transaction ID with sender's private key */
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}

