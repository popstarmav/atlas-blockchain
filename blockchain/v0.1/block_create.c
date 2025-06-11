#include "blockchain.h"

/**
 * block_create - Creates a Block structure and initializes it
 * @prev: pointer to the previous Block in the Blockchain
 * @data: points to a memory area to duplicate in the Block's data
 * @data_len: number of bytes to duplicate in data
 *
 * Return: pointer to the allocated Block, or NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
		uint32_t data_len)
{
	block_t *new_block;
	uint32_t copy_len;

	if (!prev || !data)
		return (NULL);

	new_block = malloc(sizeof(block_t));

	if (!new_block)
		return (NULL);

	/* Determine how many bytes to copy */
	copy_len = (data_len > BLOCKCHAIN_DATA_MAX) ? 
		BLOCKCHAIN_DATA_MAX : data_len;

	/* Initialize block info */
	new_block->info.index = prev->info.index + 1;
	new_block->info.difficulty = 0;
	new_block->info.nonce = 0;
	new_block->info.timestamp = time(NULL);
	memcpy(new_block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

	/* Initialize block data */
	memset(new_block->data.buffer, 0, BLOCKCHAIN_DATA_MAX);
	memcpy(new_block->data.buffer, data, copy_len);
	new_block->data.len = copy_len;

	/* Zero out the hash */
	memset(new_block->hash, 0, SHA256_DIGEST_LENGTH);

	return (new_block);
}
