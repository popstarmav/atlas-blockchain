#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * unspent_tx_out_create - Allocates and initializes
 * an unspent transaction output structure
 * @block_hash: Hash of the Block where the
 * referenced transaction output is located
 * @tx_id: Hash of a transaction in the Block block_hash
 * @out: Pointer to the referenced transaction output
 *
 * Return: Pointer to the created unspent transaction
 * output upon success, or NULL upon failure
 */
unspent_tx_out_t *unspent_tx_out_create(uint8_t block_hash
		[SHA256_DIGEST_LENGTH],uint8_t tx_id[SHA256_DIGEST_LENGTH],
                                       tx_out_t const *out)

{
	unspent_tx_out_t *unspent;

	/* Check for NULL parameters */
	if (!block_hash || !tx_id || !out)
		return (NULL);
	/* Allocate memory for the unspent transaction output */
	unspent = malloc(sizeof(unspent_tx_out_t));
	if (!unspent)
		return (NULL);

	/* Copy the block hash */
	memcpy(unspent->block_hash, block_hash, SHA256_DIGEST_LENGTH);

	/* Copy the transaction ID */
	memcpy(unspent->tx_id, tx_id, SHA256_DIGEST_LENGTH);

	/* Copy the transaction output */
	memcpy(&unspent->out, out, sizeof(tx_out_t));

	return (unspent);
}

