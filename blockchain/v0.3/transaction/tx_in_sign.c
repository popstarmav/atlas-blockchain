#include <string.h>
#include "blockchain.h"

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
	uint8_t sender_pub[EC_PUB_LEN];
	int i, list_size;

	/* Validate input parameters */
	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);

	/* Extract public key from sender's private key */
	if (!ec_to_pub(sender, sender_pub))
		return (NULL);

	/* Find the UTXO that matches the input's tx_out_hash */
	list_size = llist_size(all_unspent);
	utxo = NULL;

	for (i = 0; i < list_size; i++)
	{
		utxo = (unspent_tx_out_t *)llist_get_node_at(all_unspent, i);
		if (!utxo)
			continue;

		/* Compare the UTXO's output hash with the input's reference */
		if (memcmp(utxo->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH) == 0)
			break;
		
		utxo = NULL;
	}

	/* If no matching UTXO found, cannot sign */
	if (!utxo)
		return (NULL);

	/*
	 * CRITICAL OWNERSHIP CHECK:
	 * Compare sender's public key with the UTXO's public key
	 * If they don't match, sender doesn't own this UTXO and cannot spend it
	 */
	if (memcmp(sender_pub, utxo->out.pub, EC_PUB_LEN) != 0)
		return (NULL);

	/* Clear signature structure */
	memset(&in->sig, 0, sizeof(sig_t));

	/* Sign the transaction ID with sender's private key */
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}

