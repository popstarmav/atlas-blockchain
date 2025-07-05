#include "blockchain.h"

/**
 * tx_in_create - Allocates and initializes a transaction input structure
 * @unspent: Pointer to the unspent transaction output to be converted
 *
 * Return: Pointer to the created transaction input upon success, or NULL upon failure
 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
    tx_in_t *tx_in;

    if (!unspent)
        return (NULL);

    tx_in = calloc(1, sizeof(tx_in_t));
    if (!tx_in)
        return (NULL);

    /* Copy block hash from unspent transaction output */
    memcpy(tx_in->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);

    /* Copy transaction ID from unspent transaction output */
    memcpy(tx_in->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);

    /* Copy transaction output hash from the embedded tx_out_t structure */
    memcpy(tx_in->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);

    return (tx_in);
}

