#include <string.h>
#include <stdlib.h>
#include "blockchain.h"

/**
 * transaction_hash - Computes the ID (hash) of a transaction
 * @transaction: Pointer to the transaction to compute the hash of
 * @hash_buf: Buffer in which to store the computed hash
 *
 * Return: Pointer to hash_buf
 */
uint8_t *transaction_hash(transaction_t const *transaction,
			  uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint8_t *buffer, *ptr;
	size_t buffer_size;
	int inputs_count, outputs_count, i;
	tx_in_t *input;
	tx_out_t *output;

	if (!transaction || !hash_buf)
		return (NULL);

	inputs_count = llist_size(transaction->inputs);
	outputs_count = llist_size(transaction->outputs);

	/* Calculate buffer size: inputs (96 bytes each) + outputs (32 bytes each) */
	buffer_size = (inputs_count * (SHA256_DIGEST_LENGTH * 3)) +
		      (outputs_count * SHA256_DIGEST_LENGTH);

	buffer = malloc(buffer_size);
	if (!buffer)
		return (NULL);

	ptr = buffer;

	/* Copy input data: block_hash + tx_id + tx_out_hash for each input */
	for (i = 0; i < inputs_count; i++)
	{
		input = llist_get_node_at(transaction->inputs, i);
		if (!input)
		{
			free(buffer);
			return (NULL);
		}

		memcpy(ptr, input->block_hash, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;

		memcpy(ptr, input->tx_id, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;

		memcpy(ptr, input->tx_out_hash, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
	}

	/* Copy output data: hash for each output */
	for (i = 0; i < outputs_count; i++)
	{
		output = llist_get_node_at(transaction->outputs, i);
		if (!output)
		{
			free(buffer);
			return (NULL);
		}

		memcpy(ptr, output->hash, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
	}

	/* Compute SHA256 hash of the buffer */
	sha256((int8_t *)buffer, buffer_size, hash_buf);

	free(buffer);
	return (hash_buf);
}

