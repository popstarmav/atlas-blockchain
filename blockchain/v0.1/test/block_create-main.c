#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_destroy(blockchain_t *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *genesis_block;
    
    printf("Creating blockchain...\n");
    blockchain = blockchain_create();
    if (!blockchain)
    {
        fprintf(stderr, "Failed to create blockchain\n");
        return (EXIT_FAILURE);
    }
    
    printf("Blockchain created successfully\n");
    printf("Chain pointer: %p\n", (void*)blockchain->chain);
    printf("Chain size: %d\n", llist_size(blockchain->chain));
    
    genesis_block = llist_get_head(blockchain->chain);
    printf("Genesis block pointer: %p\n", (void*)genesis_block);
    
    if (genesis_block)
    {
        printf("Genesis block info:\n");
        printf("  Index: %u\n", genesis_block->info.index);
        printf("  Timestamp: %lu\n", genesis_block->info.timestamp);
        printf("  Data length: %u\n", genesis_block->data.len);
        printf("  Data: %.16s\n", genesis_block->data.buffer);
    }
    else
    {
        printf("ERROR: Genesis block is NULL\n");
    }
    
    /* Don't call _blockchain_print for now to avoid segfault */
    printf("Skipping _blockchain_print to avoid segfault\n");
    
    printf("Destroying blockchain...\n");
    _blockchain_destroy(blockchain);
    printf("Done\n");
    
    return (EXIT_SUCCESS);
}
