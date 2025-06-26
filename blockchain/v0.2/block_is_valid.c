#include <string.h>
#include "blockchain.h"

/**
 * block_is_valid - Validates block data
 * @block: Pointer to block
 * @prev_block: Pointer to previous block
 *
 * Return: 0 if valid, 1 if invalid (opposite of normal convention)
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
    uint8_t hash_buf[SHA256_DIGEST_LENGTH];

    if (!block)
        return (1); /* Invalid */

    /* Handle genesis block */
    if (block->info.index == 0)
    {
        if (!prev_block)
            return (memcmp(block, &_genesis, sizeof(_genesis)) != 0 ? 1 : 0);
        else
            return (1); /* Genesis should not have prev_block */
    }

    /* Non-genesis block must have prev_block */
    if (!prev_block)
        return (1); /* Invalid */

    /* Check index sequence */
    if (block->info.index != prev_block->info.index + 1)
        return (1); /* Invalid */

    /* Verify previous block's hash matches what we store */
    if (!block_hash(prev_block, hash_buf))
        return (1); /* Invalid */

    if (memcmp(hash_buf, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
        return (1); /* Invalid */

    /* Check if prev_hash in current block matches previous block's hash */
    if (memcmp(prev_block->hash, block->info.prev_hash, SHA256_DIGEST_LENGTH) != 0)
        return (1); /* Invalid */

    /* Verify current block's hash */
    if (!block_hash(block, hash_buf))
        return (1); /* Invalid */

    if (memcmp(hash_buf, block->hash, SHA256_DIGEST_LENGTH) != 0)
        return (1); /* Invalid */

    /* Check if hash matches difficulty */
    if (!hash_matches_difficulty(block->hash, block->info.difficulty))
        return (1); /* Invalid */

    return (0); /* Valid */
}

