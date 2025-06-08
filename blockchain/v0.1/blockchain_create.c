#include "blockchain.h"

blockchain_t *blockchain_create(void)
{
	static block_t genesis_block = {
		{0, 0, 1537578000, 0, {0}}, /* info */
		{"Holberton School", 16}, /* data */
		{0xc5, 0x2c, 0x26, 0xc8, 0xb5, 0x46, 0x16, 0x39,
		 0x63, 0x5d, 0x8e, 0xdf, 0x2a, 0x97, 0xd4, 0x8d,
		 0x0c, 0x8e, 0x00, 0x09, 0xc8, 0x17, 0xf2, 0xb1,
		 0xd3, 0xd7, 0xff, 0x2f, 0x04, 0x51, 0x58, 0x03} /* hash */
	};
	
	blockchain_t *blockchain;
	llist_t *genesis_node;

	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);

	genesis_node = malloc(sizeof(llist_t));
	if (!genesis_node)
	{
		free(blockchain);
		return (NULL);
	}

	genesis_node->node = &genesis_block;
	genesis_node->next = NULL;
	genesis_node->prev = NULL;

	blockchain->chain = genesis_node;
	return (blockchain);
}
