#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "transaction.h"

/**
 * tx_out_create - Creates a transaction output
 * @amount: Amount of the transaction
 * @pub: Public key of the transaction receiver
 *
 * Return: Pointer to the created transaction output, or NULL on failure
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
    tx_out_t *tx_out;
    SHA256_CTX sha256_ctx;
    
    if (!pub)
        return (NULL);
    
    /* Allocate memory for transaction output */
    tx_out = malloc(sizeof(tx_out_t));
    if (!tx_out)
        return (NULL);
    
    /* Initialize transaction output */
    tx_out->amount = amount;
    memcpy(tx_out->pub, pub, EC_PUB_LEN);
    
    /* Compute hash of amount and public key */
    if (!SHA256_Init(&sha256_ctx))
    {
        free(tx_out);
        return (NULL);
    }
    
    if (!SHA256_Update(&sha256_ctx, &tx_out->amount, sizeof(tx_out->amount)))
    {
        free(tx_out);
        return (NULL);
    }
    
    if (!SHA256_Update(&sha256_ctx, tx_out->pub, EC_PUB_LEN))
    {
        free(tx_out);
        return (NULL);
    }
    
    if (!SHA256_Final(tx_out->hash, &sha256_ctx))
    {
        free(tx_out);
        return (NULL);
    }
    
    return (tx_out);
}

