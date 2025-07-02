#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <stdint.h>
#include <openssl/sha.h>
#include "hblk_crypto.h"

/**
 * struct tx_out_s - Transaction output
 * @amount: Amount in the transaction output
 * @pub:    Receiver's public key
 * @hash:   Hash of @amount and @pub
 */
typedef struct tx_out_s
{
    uint32_t amount;
    uint8_t  pub[EC_PUB_LEN];
    uint8_t  hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/* Function prototypes */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

#endif /* _TRANSACTION_H_ */

