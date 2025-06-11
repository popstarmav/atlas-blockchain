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
    block_t *new_block = NULL;
    block_info_t info;
    block_data_t new_data = {{0}, 0};
    uint32_t len = data_len;

    if (!prev || !data)
        return (NULL);

    new_block = malloc(sizeof(block_t));
    if (!new_block)
        return (NULL);

    /* Limit data length to maximum allowed */
    if (data_len > BLOCKCHAIN_DATA_MAX)
        len = BLOCKCHAIN_DATA_MAX;

    /* Initialize block data */
    memcpy(new_data.buffer, data, len);
    new_data.len = len;

    /* Initialize block info */
    info.index = prev->info.index + 1;
    info.difficulty = 0;
    info.nonce = 0;
    memcpy(info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
    info.timestamp = time(NULL);

    /* Zero out the hash */
    memset(new_block->hash, 0, SHA256_DIGEST_LENGTH);

    /* Assign structures to new block */
    new_block->data = new_data;
    new_block->info = info;

    return (new_block);
}

