#include "blockchain.h"

/**
 * block_destroy - Deallocates a Block
 * @block: Pointer to the Block to delete
 */
void block_destroy(block_t *block)
{
    if (!block)
        return;

    if (block->data.buffer)
        free(block->data.buffer);
    
    free(block);
}

