#include "blockchain.h"

/**
 * _blockchain_destroy - Destroys a blockchain and all its blocks
 * @blockchain: Pointer to the blockchain to destroy
 */
void _blockchain_destroy(blockchain_t *blockchain)
{
    if (!blockchain)
        return;

    if (blockchain->chain)
        llist_destroy(blockchain->chain, 1, (void (*)(void *))block_destroy);

    free(blockchain);
}

