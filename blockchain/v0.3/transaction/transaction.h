#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <stdint.h>
#include <openssl/sha.h>
#include <llist.h>

/* Suppress OpenSSL deprecation warnings */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "hblk_crypto.h"
#pragma GCC diagnostic pop

#define COINBASE_AMOUNT 50

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

/** 
 * struct tx_in_s - Transaction input
 * @block_hash:  Hash of the Block containing the transaction
 * @tx_id:       ID of the transaction containing @tx_out_hash
 * @tx_out_hash: Hash of the referenced transaction output
 * @sig:         Signature of the transaction input
 */
typedef struct tx_in_s
{
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t tx_id[SHA256_DIGEST_LENGTH];
    uint8_t tx_out_hash[SHA256_DIGEST_LENGTH];
    sig_t   sig;
} tx_in_t;

/** 
 * struct transaction_s - Transaction structure
 * @id:      Transaction ID
 * @inputs:  List of transaction inputs
 * @outputs: List of transaction outputs
 */
typedef struct transaction_s
{
    uint8_t id[SHA256_DIGEST_LENGTH];
    llist_t *inputs;
    llist_t *outputs;
} transaction_t;

/** 
 * struct unspent_tx_out_s - Unspent transaction output
 * @block_hash: Hash of the Block containing the transaction
 * @tx_id:      ID of the transaction containing @out
 * @out:        Copy of the referenced transaction output
 */
typedef struct unspent_tx_out_s
{
    uint8_t   block_hash[SHA256_DIGEST_LENGTH];
    uint8_t   tx_id[SHA256_DIGEST_LENGTH];
    tx_out_t  out;
} unspent_tx_out_t;

/**
 * struct tx_data_s - Helper structure for transaction creation
 * @pub: Sender's public key
 * @needed: Amount needed for the transaction
 * @amount_total: Total amount collected from UTXOs
 * @txt: Pointer to the transaction being built
 * @sender: Sender's private key
 * @all_unspent: List of all unspent outputs
 */
typedef struct tx_data_s
{
    uint8_t pub[EC_PUB_LEN];
    uint32_t needed;
    uint32_t amount_total;
    transaction_t *txt;
    EC_KEY const *sender;
    llist_t *all_unspent;
} tx_data_t;

/* Function prototypes */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

/* Print functions */
void _transaction_print(transaction_t const *transaction);
void _transaction_print_brief(transaction_t const *transaction);
int _transaction_print_loop(transaction_t const *transaction,
    unsigned int idx, char const *indent);
int _transaction_print_brief_loop(transaction_t const *transaction,
    unsigned int idx, char const *indent);
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);

unspent_tx_out_t *unspent_tx_out_create(uint8_t block_hash[SHA256_DIGEST_LENGTH],
                uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out);
uint8_t *transaction_hash(transaction_t const *transaction, 
		uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH], 
		EC_KEY const *sender, llist_t *all_unspent);
transaction_t *transaction_create(EC_KEY const *sender,
		EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent);

#endif /* _TRANSACTION_H_ */

