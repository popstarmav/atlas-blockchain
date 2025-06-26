#include "blockchain.h"

/**
 * block_hash - Computes the hash of a Block
 * @block: Block to hash
 * @hash_buf: Buffer to store the computed hash
 *
 * Return: Pointer to hash_buf, or NULL on failure
 */
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
    if (!block || !hash_buf)
        return (NULL);

    return (sha256((int8_t const *)block, sizeof(block->info) + block->data.len, hash_buf));
}

