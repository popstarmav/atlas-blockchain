#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* SHA256 digest length */
#define SHA256_DIGEST_LENGTH 32

/* Block data buffer size */
#define BLOCKCHAIN_DATA_MAX 1024

/**
 * struct llist_s - Linked list node structure (used as both list and node)
 * @node: Pointer to the stored data
 * @next: Pointer to the next node in the list
 * @prev: Pointer to the previous node in the list (if needed)
 */
typedef struct llist_s
{
	void *node;
	struct llist_s *next;
	struct llist_s *prev;
} llist_t;

/**
 * struct block_info_s - Block info structure
 * @index: Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp: Time the Block was created at (UNIX timestamp)
 * @nonce: Salt value used to alter the Block hash
 * @prev_hash: Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
	uint32_t index;
	uint32_t difficulty;
	uint64_t timestamp;
	uint64_t nonce;
	uint8_t prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_data_s - Block data structure
 * @buffer: Data buffer
 * @len: Data size (in bytes)
 */
typedef struct block_data_s
{
	int8_t buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t len;
} block_data_t;

/**
 * struct block_s - Block structure
 * @info: Block info
 * @data: Block data
 * @hash: 256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t info;
	block_data_t data;
	uint8_t hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_s - Blockchain structure
 * @chain: Linked list of pointers to block_t
 */
typedef struct blockchain_s
{
	llist_t *chain;
} blockchain_t;

/* Linked list function prototypes */
llist_t *llist_create(int mt_support);
int llist_destroy(llist_t *list, int free_nodes, void (*free_node)(void *));
int llist_add_node(llist_t *list, void *node, int mode);
void *llist_get_head(llist_t *list);
void *llist_get_tail(llist_t *list);

/* Linked list modes */
#define ADD_NODE_FRONT 0
#define ADD_NODE_REAR 1
#define MT_SUPPORT_FALSE 0
#define MT_SUPPORT_TRUE 1

/* Function prototypes */
blockchain_t *blockchain_create(void);

#endif /* BLOCKCHAIN_H */

