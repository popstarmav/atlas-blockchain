#include "blockchain.h"

/**
 * block_create - Creates a Block
 * @prev: Pointer to the previous Block in the Blockchain
 * @data: Data to be saved in the Block
 * @data_len: Number of bytes in data
 *
 * Return: Pointer to the created Block, or NULL upon failure
 */
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len)
{
    block_t *block;

    if (!prev || !data)
        return (NULL);

    block = calloc(1, sizeof(*block));
    if (!block)
        return (NULL);

    block->info.index = prev->info.index + 1;
    block->info.difficulty = 0;
    block->info.timestamp = time(NULL);
    block->info.nonce = 0;
    memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

    block->data.len = data_len;
    memcpy(block->data.buffer, data, data_len);

    if (!block_hash(block, block->hash))
    {
        free(block);
        return (NULL);
    }

    return (block);
}

