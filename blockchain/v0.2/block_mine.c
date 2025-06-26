#include "blockchain.h"

/**
 * block_mine - Mines a Block in order to insert it in the Blockchain
 * @block: Pointer to the Block to be mined
 *
 * Description: This function finds a hash for block that matches its difficulty
 * by incrementing the nonce until the hash has the required number of leading
 * zero bits.
 */
void block_mine(block_t *block)
{
    uint8_t hash_buf[SHA256_DIGEST_LENGTH];

    if (!block)
        return;

    /* Keep incrementing nonce until we find a hash that matches difficulty */
    while (1)
    {
        /* Update timestamp for each attempt */
        block->info.timestamp = time(NULL);
        
        /* Compute hash with current nonce */
        if (!block_hash(block, hash_buf))
            return;

        /* Check if hash matches difficulty */
        if (hash_matches_difficulty(hash_buf, block->info.difficulty))
        {
            /* Found a valid hash, copy it to block */
            memcpy(block->hash, hash_buf, SHA256_DIGEST_LENGTH);
            break;
        }

        /* Increment nonce and try again */
        block->info.nonce++;
    }
}

