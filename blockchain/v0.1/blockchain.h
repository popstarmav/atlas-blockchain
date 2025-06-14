#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_

#include <openssl/sha.h>
#include <llist.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define BLOCKCHAIN_DATA_MAX 1024
#define SHA256_DIGEST_LENGTH 32

#define ATLAS_HASH \
    "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
    "\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"

typedef int (*node_func_t)(void *, unsigned int, void *);

/* Your existing typedefs are perfect - keep them as is */
typedef struct block_info_s
{
    uint32_t    index;
    uint32_t    difficulty;
    uint64_t    timestamp;
    uint64_t    nonce;
    uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

typedef struct block_data_s
{
    int8_t      buffer[BLOCKCHAIN_DATA_MAX];
    uint32_t    len;
} block_data_t;

typedef struct block_s
{
    block_info_t    info;
    block_data_t    data;
    uint8_t         hash[SHA256_DIGEST_LENGTH];
} block_t;

typedef struct blockchain_s
{
    llist_t     *chain;
} blockchain_t;

/* Function prototypes - your list is complete and correct */
blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len);
void block_destroy(block_t *block);
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
void blockchain_destroy(blockchain_t *blockchain);

#endif /* _BLOCKCHAIN_H_ */

