#include "blockchain.h"
#include <stdlib.h>

/**
 * _blockchain_destroy - Destroys a blockchain
 * @blockchain: Pointer to the blockchain to destroy
 */
void _blockchain_destroy(blockchain_t *blockchain)
{
    if (!blockchain)
        return;
        
    if (blockchain->chain)
        llist_destroy(blockchain->chain, 1, NULL);
        
    free(blockchain);
}
