#include <string.h>
#include "blockchain.h"

/**
 * find_matching_utxo - Searches for unspent output matching the input
 * @utxo_list: List of unspent transaction outputs
 * @target_hash: Hash of the output we're looking for
 *
 * Return: Pointer to matching unspent output, or NULL if not found
 */
static unspent_tx_out_t *find_matching_utxo(llist_t *utxo_list,
                                             uint8_t *target_hash)
{
	unspent_tx_out_t *current_utxo;
	int list_length, index;

	if (!utxo_list || !target_hash)
		return (NULL);

	list_length = llist_size(utxo_list);
	for (index = 0; index < list_length; index++)
	{
		current_utxo = (unspent_tx_out_t *)llist_get_node_at(utxo_list, index);
		if (!current_utxo)
			continue;

		if (memcmp(current_utxo->out.hash, target_hash,
			   SHA256_DIGEST_LENGTH) == 0)
			return (current_utxo);
	}
	return (NULL);
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
	unspent_tx_out_t *referenced_output;
	uint8_t sender_pub[EC_PUB_LEN];

	/* Validate all required parameters */
	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);

	/* Locate the unspent output referenced by this input */
	referenced_output = find_matching_utxo(all_unspent, in->tx_out_hash);
	if (!referenced_output)
		return (NULL);

	/* Extract public key from sender's private key */
	if (!ec_to_pub(sender, sender_pub))
		return (NULL);

	/* Verify sender owns the referenced output by comparing public keys */
	if (memcmp(sender_pub, referenced_output->out.pub, EC_PUB_LEN) != 0)
		return (NULL);

	/* Initialize signature structure to zero */
	memset(&in->sig, 0, sizeof(sig_t));

	/* Create digital signature for the transaction using ec_sign */
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}

