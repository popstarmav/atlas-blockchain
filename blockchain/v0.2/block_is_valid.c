#include "blockchain.h"

/**
 * block_is_valid - Verifies that a Block is valid
 * @block: Pointer to the Block to check
 * @prev_block: Pointer to the previous Block in the Blockchain
 *
 * Return: 1 if valid, 0 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
    uint8_t hash_buf[SHA256_DIGEST_LENGTH];

    if (!block)
        return (0);

    /* Genesis block validation */
    if (!prev_block)
    {
        /* Should be the genesis block */
        if (memcmp(block, &_genesis, sizeof(_genesis)) != 0)
            return (0);
        return (1);
    }

    /* Check if index is correct */
    if (block->info.index != prev_block->info.index + 1)
        return (0);

    /* Check if prev_hash matches previous block's hash */
    if (memcmp(block->info.prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
        return (0);

    /* Verify the block's hash */
    if (!block_hash(block, hash_buf))
        return (0);

    if (memcmp(block->hash, hash_buf, SHA256_DIGEST_LENGTH) != 0)
        return (0);

    /* Check if hash matches difficulty */
    if (!hash_matches_difficulty(block->hash, block->info.difficulty))
        return (0);

    return (1);
}

