#include "blockchain.h"

/**
 * block_is_valid - Verifies that a Block is valid
 * @block: Block to verify
 * @prev_block: Previous Block in the Blockchain
 *
 * Return: 1 if valid, 0 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
    uint8_t hash_buf[SHA256_DIGEST_LENGTH];
    int i;

    if (!block)
        return (0);

    /* Genesis Block validation */
    if (!prev_block)
    {
        if (memcmp(block, &_genesis, sizeof(_genesis)) != 0)
            return (0);
        return (1);
    }

    /* Check if previous hash matches */
    if (memcmp(block->info.prev_hash, prev_block->hash,
               SHA256_DIGEST_LENGTH) != 0)
        return (0);

    /* Check if block index is correct */
    if (block->info.index != prev_block->info.index + 1)
        return (0);

    /* Verify block hash */
    if (!block_hash(block, hash_buf) ||
        memcmp(block->hash, hash_buf, SHA256_DIGEST_LENGTH) != 0)
        return (0);

    /* Check if hash matches difficulty (leading zeros) */
    for (i = 0; i < block->info.difficulty; i++)
    {
        if (block->hash[i] != 0)
            return (0);
    }

    return (1);
}

