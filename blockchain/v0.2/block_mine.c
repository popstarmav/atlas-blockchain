#include "blockchain.h"

/**
 * block_mine - Mines a Block in order to insert it in the Blockchain
 * @block: Pointer to the Block to be mined
 */
void block_mine(block_t *block)
{
    uint8_t hash_buf[SHA256_DIGEST_LENGTH];
    
    if (!block)
        return;
    
    for (block->info.nonce = 0; block->info.nonce != UINT64_MAX; block->info.nonce++)
    {
        if (!block_hash(block, hash_buf))
            return;
            
        if (hash_matches_difficulty(hash_buf, block->info.difficulty))
        {
            /* Copy the valid hash to the block */
            memcpy(block->hash, hash_buf, SHA256_DIGEST_LENGTH);
            return;
        }
    }
}

