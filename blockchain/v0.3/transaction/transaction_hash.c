#include <string.h>
#include <stdlib.h>
#include "blockchain.h"

/**
 * calculate_buffer_size - Calculates the size needed for the hash buffer
 * @transaction: Pointer to the transaction
 *
 * Return: Size of the buffer needed
 */
static size_t calculate_buffer_size(transaction_t const *transaction)
{
	int inputs_count, outputs_count;

	inputs_count = llist_size(transaction->inputs);
	outputs_count = llist_size(transaction->outputs);

	/* Calculate buffer size: inputs (96 bytes each) + outputs (32 bytes each) */
	return ((inputs_count * (SHA256_DIGEST_LENGTH * 3)) +
		(outputs_count * SHA256_DIGEST_LENGTH));
}

/**
 * copy_inputs_data - Copies input data to the buffer
 * @transaction: Pointer to the transaction
 * @buffer: Buffer to copy data to
 *
 * Return: Pointer to the next position in buffer after copied data,
 * or NULL on failure
 */
static uint8_t *copy_inputs_data(transaction_t const *transaction, uint8_t *buffer)
{
	tx_in_t *input;
	int i, inputs_count;
	uint8_t *ptr = buffer;

	inputs_count = llist_size(transaction->inputs);

	/* Copy input data: block_hash + tx_id + tx_out_hash for each input */
	for (i = 0; i < inputs_count; i++)
	{
		input = llist_get_node_at(transaction->inputs, i);
		if (!input)
			return (NULL);

		memcpy(ptr, input->block_hash, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
		memcpy(ptr, input->tx_id, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
		memcpy(ptr, input->tx_out_hash, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
	}

	return (ptr);
}

/**
 * copy_outputs_data - Copies output data to the buffer
 * @transaction: Pointer to the transaction
 * @buffer: Buffer to copy data to
 *
 * Return: 1 on success, 0 on failure
 */
static int copy_outputs_data(transaction_t const *transaction, uint8_t *buffer)
{
	tx_out_t *output;
	int i, outputs_count;
	uint8_t *ptr = buffer;

	outputs_count = llist_size(transaction->outputs);

	/* Copy output data: hash for each output */
	for (i = 0; i < outputs_count; i++)
	{
		output = llist_get_node_at(transaction->outputs, i);
		if (!output)
			return (0);

		memcpy(ptr, output->hash, SHA256_DIGEST_LENGTH);
		ptr += SHA256_DIGEST_LENGTH;
	}

	return (1);
}

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

	if (!transaction || !hash_buf)
		return (NULL);

	/* Calculate buffer size */
	buffer_size = calculate_buffer_size(transaction);

	buffer = malloc(buffer_size);
	if (!buffer)
		return (NULL);

	/* Copy input data */
	ptr = copy_inputs_data(transaction, buffer);
	if (!ptr)
	{
		free(buffer);
		return (NULL);
	}

	/* Copy output data */
	if (!copy_outputs_data(transaction, ptr))
	{
		free(buffer);
		return (NULL);
	}

	/* Compute SHA256 hash of the buffer */
	sha256((int8_t *)buffer, buffer_size, hash_buf);
	free(buffer);

	return (hash_buf);
}

