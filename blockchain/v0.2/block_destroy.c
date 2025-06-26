#include "blockchain.h"

/**
 * block_destroy - Deallocates a Block
 * @block: Pointer to the Block to delete
 */
void block_destroy(block_t *block)
{
    if (!block)
        return;

    /* Only free data buffer if it's not the genesis block */
    /* Genesis block has index 0 and static data */
    if (block->info.index != 0 && block->data.buffer)
    {
        free(block->data.buffer);
    }
    
    /* Free the block itself (but not if it's the static genesis block) */
    if (block != &_genesis)
        free(block);
}

