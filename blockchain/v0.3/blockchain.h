#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_

#include <llist.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCKCHAIN_DATA_MAX 1024

/**
 * struct block_info_s - Block info structure
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
    uint32_t    index;
    uint32_t    difficulty;
    uint64_t    timestamp;
    uint64_t    nonce;
    uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_data_s - Block data
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_s
{
    char        *buffer;  /* Changed from void* to char* */
    uint32_t    len;
} block_data_t;

/**
 * struct block_s - Block structure
 * @info:         Block info
 * @data:         Block data
 * @transactions: List of transactions
 * @hash:         256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
    block_info_t info; /* This must stay first */
    block_data_t data; /* This must stay second */
    llist_t     *transactions; /* List of transactions */
    uint8_t     hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_s - Blockchain structure
 * @chain:   Linked list of Blocks
 * @unspent: List of unspent transaction outputs
 */
typedef struct blockchain_s
{
    llist_t *chain;
    llist_t *unspent;
} blockchain_t;

/* Include transaction types */
#include "transaction/transaction.h"

/* Genesis Block */
extern block_t const _genesis;

/* Function prototypes */
void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_print_brief(blockchain_t const *blockchain);

/* Block functions */
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len);
void block_destroy(block_t *block);

#endif /* _BLOCKCHAIN_H_ */

