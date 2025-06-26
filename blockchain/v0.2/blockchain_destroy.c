#include "blockchain.h"

/**
 * blockchain_destroy - Deallocates a Blockchain structure
 * @blockchain: Pointer to the Blockchain structure to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
    if (!blockchain)
        return;

    /* Destroy the linked list and all blocks */
    if (blockchain->chain)
        llist_destroy(blockchain->chain, 1, (node_dtor_t)block_destroy);

    /* Free the blockchain structure */
    free(blockchain);
}

