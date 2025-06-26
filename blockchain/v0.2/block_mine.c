#include "blockchain.h"

/**
 * block_mine - Mines a block by finding a nonce that satisfies the difficulty
 * @block: Pointer to the Block to be mined
 *
 * Description: This function increments the nonce until the block's hash
 * matches the required difficulty (number of leading zero bits).
 */
void block_mine(block_t *block)
{
    if (!block)
        return;

    /* Start with nonce = 0 and increment until we find a valid hash */
    block->info.nonce = 0;

    /* Keep trying different nonce values until hash matches difficulty */
    while (1)
    {
        /* Compute hash with current nonce */
        if (!block_hash(block, block->hash))
            return;

        /* Check if hash matches the required difficulty */
        if (hash_matches_difficulty(block->hash, block->info.difficulty))
            break;

        /* Increment nonce and try again */
        block->info.nonce++;
    }
}

