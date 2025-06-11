#include "blockchain.h"

/**
 * block_hash - Computes the hash of a Block
 * @block: Pointer to the Block to be hashed
 * @hash_buf: Buffer to store the resulting hash
 *
 * Return: Pointer to hash_buf, or NULL on failure
 */
uint8_t *block_hash(block_t const *block,
                    uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
    size_t info_size, data_size, total_size;
    uint8_t *temp_buffer;

    if (!block || !hash_buf)
        return (NULL);

    info_size = sizeof(block->info);
    data_size = block->data.len;
    total_size = info_size + data_size;

    /* Create temporary buffer to concatenate info + data */
    temp_buffer = malloc(total_size);
    if (!temp_buffer)
        return (NULL);

    /* Copy block info */
    memcpy(temp_buffer, &(block->info), info_size);
    
    /* Copy actual data (only the used portion) */
    memcpy(temp_buffer + info_size, block->data.buffer, data_size);

    /* Compute SHA256 hash */
    SHA256(temp_buffer, total_size, hash_buf);

    free(temp_buffer);
    return (hash_buf);
}

