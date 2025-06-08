#include "blockchain.h"

/**
 * blockchain_create - Creates a Blockchain structure and initializes it
 *
 * Return: Pointer to the created blockchain, or NULL on failure
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain;
	block_t *genesis_block;
	uint8_t genesis_hash[] = {
		0xc5, 0x2c, 0x26, 0xc8, 0xb5, 0x46, 0x16, 0x39,
		0x63, 0x5d, 0x8e, 0xdf, 0x2a, 0x97, 0xd4, 0x8d,
		0x0c, 0x8e, 0x00, 0x09, 0xc8, 0x17, 0xf2, 0xb1,
		0xd3, 0xd7, 0xff, 0x2f, 0x04, 0x51, 0x58, 0x03
	};

	/* Allocate memory for blockchain */
	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);

	/* Create linked list for the chain */
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}

	/* Allocate memory for genesis block */
	genesis_block = malloc(sizeof(block_t));
	if (!genesis_block)
	{
		llist_destroy(blockchain->chain, 0, NULL);
		free(blockchain);
		return (NULL);
	}

	/* Initialize genesis block info */
	genesis_block->info.index = 0;
	genesis_block->info.difficulty = 0;
	genesis_block->info.timestamp = 1537578000;
	genesis_block->info.nonce = 0;
	memset(genesis_block->info.prev_hash, 0, SHA256_DIGEST_LENGTH);

	/* Initialize genesis block data */
	memcpy(genesis_block->data.buffer, "Holberton School", 16);
	genesis_block->data.len = 16;

	/* Set genesis block hash */
	memcpy(genesis_block->hash, genesis_hash, SHA256_DIGEST_LENGTH);

	/* Add genesis block to the chain */
	if (llist_add_node(blockchain->chain, genesis_block, ADD_NODE_REAR) != 0)
	{
		free(genesis_block);
		llist_destroy(blockchain->chain, 0, NULL);
		free(blockchain);
		return (NULL);
	}

	return (blockchain);
}

