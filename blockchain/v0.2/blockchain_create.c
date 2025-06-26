#include "blockchain.h"

/**
 * blockchain_create - Creates a Blockchain structure and initializes it
 *
 * Return: Pointer to the created Blockchain, or NULL upon failure
 */
blockchain_t *blockchain_create(void)
{
    blockchain_t *blockchain;
    block_t *genesis;

    blockchain = calloc(1, sizeof(*blockchain));
    if (!blockchain)
        return (NULL);

    blockchain->chain = llist_create(MT_SUPPORT_FALSE);
    if (!blockchain->chain)
    {
        free(blockchain);
        return (NULL);
    }

    genesis = calloc(1, sizeof(*genesis));
    if (!genesis)
    {
        llist_destroy(blockchain->chain, 0, NULL);
        free(blockchain);
        return (NULL);
    }

    /* Copy the genesis block */
    memcpy(genesis, &_genesis, sizeof(_genesis));

    if (llist_add_node(blockchain->chain, genesis, ADD_NODE_REAR) != 0)
    {
        free(genesis);
        llist_destroy(blockchain->chain, 0, NULL);
        free(blockchain);
        return (NULL);
    }

    return (blockchain);
}
