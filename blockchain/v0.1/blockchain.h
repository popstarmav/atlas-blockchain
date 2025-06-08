#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SHA256_DIGEST_LENGTH 32
#define BLOCKCHAIN_DATA_MAX 1024

/**
 * struct llist_s - Linked list node structure
 * @node: Pointer to the stored data
 * @next: Pointer to the next node
 * @prev: Pointer to the previous node
 */
typedef struct llist_s
{
	void *node;
	struct llist_s *next;
	struct llist_s *prev;
} llist_t;

typedef struct block_info_s
{
	uint32_t index;
	uint32_t difficulty;
	uint64_t timestamp;
	uint64_t nonce;
	uint8_t prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

typedef struct block_data_s
{
	int8_t buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t len;
} block_data_t;

typedef struct block_s
{
	block_info_t info;
	block_data_t data;
	uint8_t hash[SHA256_DIGEST_LENGTH];
} block_t;

typedef struct blockchain_s
{
	llist_t *chain;
} blockchain_t;

/* Linked list function prototypes */
llist_t *llist_create(int mt_support);
int llist_destroy(llist_t *list, int free_nodes, void (*free_node)(void *));
void *llist_get_head(llist_t *list);
int llist_size(llist_t *list);
int llist_for_each(llist_t *list, int (*func)(void *, unsigned int, void *), void *arg);

/* Callback types */
typedef int (*node_func_t)(void *, unsigned int, void *);
typedef void (*node_dtor_t)(void *);

/* Function prototypes */
blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len);

#endif
