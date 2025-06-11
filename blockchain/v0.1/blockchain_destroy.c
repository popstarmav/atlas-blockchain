#include "blockchain.h"

/**
 * blockchain_destroy - Destroys a blockchain and frees all memory
 * @blockchain: Pointer to the blockchain to destroy
 */
void blockchain_destroy(blockchain_t *blockchain)
{
    if (!blockchain)
        return;

    if (blockchain->chain)
    {
        /* Pass block_destroy as destructor to properly free each block */
        llist_destroy(blockchain->chain, 1, (void (*)(void *))block_destroy);
    }

    free(blockchain);
}
