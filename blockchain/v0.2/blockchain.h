#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "hblk_crypto.h"
#include "llist.h"

#define SHA256_DIGEST_LENGTH 32

/**
 * struct block_info_s - Block info structure
 *
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
 *
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_s
{
    void    *buffer;
    uint32_t len;
} block_data_t;

/**
 * struct block_s - Block structure
 *
 * @info: Block info
 * @data: Block data
 * @hash: 256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
    block_info_t info; /* This must stay first */
    block_data_t data; /* This must stay second */
    uint8_t      hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_s - Blockchain structure
 *
 * @chain: Linked list of pointers to block_t
 */
typedef struct blockchain_s
{
    llist_t *chain;
} blockchain_t;

/* Genesis block declaration */
extern const block_t _genesis;

/* Function prototypes */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
                            uint32_t difficulty);

/* Other function prototypes */
blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(block_t const *block,
                    uint8_t digest[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block);
void block_mine(block_t *block);
int blockchain_is_valid(blockchain_t const *blockchain);
void _blockchain_destroy(blockchain_t *blockchain);
void _blockchain_print_brief(blockchain_t const *blockchain);

#endif /* BLOCKCHAIN_H */

