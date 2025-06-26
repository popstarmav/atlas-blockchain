#include "blockchain.h"

/**
 * block_create - Creates a Block structure and initializes it
 * @prev: Pointer to the previous Block in the Blockchain
 * @data: Pointer to a memory area to duplicate in the Block
 * @data_len: Number of bytes to duplicate in @data
 *
 * Return: Pointer to the created Block, or NULL upon failure
 */
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len)
{
    block_t *new_block;

    if (!prev)
        return (NULL);

    new_block = malloc(sizeof(block_t));
    if (!new_block)
        return (NULL);

    /* Initialize block info */
    new_block->info.index = prev->info.index + 1;
    new_block->info.difficulty = 0;
    new_block->info.timestamp = time(NULL);
    new_block->info.nonce = 0;

    /* Copy previous block's hash to prev_hash */
    memcpy(new_block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

    /* Handle data */
    if (data && data_len > 0)
    {
        new_block->data.buffer = malloc(data_len);
        if (!new_block->data.buffer)
        {
            free(new_block);
            return (NULL);
        }
        memcpy(new_block->data.buffer, data, data_len);
        new_block->data.len = data_len;
    }
    else
    {
        new_block->data.buffer = NULL;
        new_block->data.len = 0;
    }

    /* Initialize hash to zeros */
    memset(new_block->hash, 0, SHA256_DIGEST_LENGTH);

    /* Calculate the block hash */
    if (!block_hash(new_block, new_block->hash))
    {
        if (new_block->data.buffer)
            free(new_block->data.buffer);
        free(new_block);
        return (NULL);
    }

    return (new_block);
}

