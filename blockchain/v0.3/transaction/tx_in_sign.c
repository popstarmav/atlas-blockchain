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
	uint8_t sender_pub[EC_PUB_LEN];
	unspent_tx_out_t *unspent;
	int i, list_size;
	int found_match = 0;

	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);

	/* Get public key from sender's private key */
	if (!ec_to_pub(sender, sender_pub))
		return (NULL);

	/* Get the size of the list */
	list_size = llist_size(all_unspent);

	/* Find the unspent transaction output that matches this input */
	for (i = 0; i < list_size; i++)
	{
		unspent = (unspent_tx_out_t *)llist_get_node_at(all_unspent, i);
		if (!unspent)
			continue;

		/* Check if this unspent output matches our input */
		if (memcmp(unspent->block_hash, in->block_hash, (size_t)SHA256_DIGEST_LENGTH) == 0 &&
		    memcmp(unspent->tx_id, in->tx_id, (size_t)SHA256_DIGEST_LENGTH) == 0 &&
		    memcmp(unspent->out.hash, in->tx_out_hash, (size_t)SHA256_DIGEST_LENGTH) == 0)
		{
			found_match = 1;
			/* Verify that sender's public key matches the output's public key */
			if (memcmp(sender_pub, unspent->out.pub, (size_t)EC_PUB_LEN) != 0)
				return (NULL);
			break;
		}
	}

	/* If no matching unspent output found, return NULL */
	if (!found_match)
		return (NULL);

	/* Sign the transaction ID */
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}

