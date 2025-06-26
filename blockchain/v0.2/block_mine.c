#include "blockchain.h"

/**
 * block_mine - Mines a block by finding a nonce that satisfies the difficulty
 * @block: Pointer to the Block to be mined
 *
 * Description: This function increments the nonce until the block's hash
 * matches the required difficulty (number of leading zero bits).
 * The mining process involves:
 * 1. Starting with nonce = 0
 * 2. Computing the block hash with current nonce
 * 3. Checking if hash matches difficulty requirement
 * 4. If not, increment nonce and repeat
 */
void block_mine(block_t *block)
{
    if (!block)
        return;

    /* Initialize nonce to 0 */
    block->info.nonce = 0;

    /* Keep trying different nonce values until hash matches difficulty */
    while (1)
    {
        /* Compute hash with current nonce value */
        if (!block_hash(block, block->hash))
            return;

        /* Check if the computed hash matches the required difficulty */
        if (hash_matches_difficulty(block->hash, block->info.difficulty))
            break;

        /* Increment nonce and try again */
        block->info.nonce++;
    }
}

