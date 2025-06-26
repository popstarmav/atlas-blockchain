#include <string.h>
#include "blockchain.h"

/**
 * block_is_valid - Validates block data
 * @block: Pointer to block  
 * @prev_block: Pointer to previous block
 *
 * Return: 0 if valid, 1 if invalid
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
    uint8_t hash_buf[SHA256_DIGEST_LENGTH];

    if (!block)
        return (1);

    /* Genesis block case */
    if (block->info.index == 0)
    {
        return (memcmp(block, &_genesis, sizeof(_genesis)) == 0 ? 0 : 1);
    }

    /* Non-genesis blocks must have prev_block */
    if (!prev_block)
        return (1);

    /* Check index sequence */
    if (block->info.index != prev_block->info.index + 1)
        return (1);

    /* Compute previous block's hash and verify it matches stored hash */
    if (!block_hash(prev_block, hash_buf))
        return (1);

    if (memcmp(hash_buf, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
        return (1);

    /* Check prev_hash in current block matches previous block's hash */
    if (memcmp(prev_block->hash, block->info.prev_hash, SHA256_DIGEST_LENGTH) != 0)
        return (1);

    /* Verify current block's computed hash matches stored hash */
    if (!block_hash(block, hash_buf))
        return (1);

    if (memcmp(hash_buf, block->hash, SHA256_DIGEST_LENGTH) != 0)
        return (1);

    /* Check if hash matches difficulty */
    if (!hash_matches_difficulty(block->hash, block->info.difficulty))
        return (1);

    return (0); /* Valid */
}

