#include "blockchain.h"
#include <stdio.h>

/**
 * _blockchain_print - Prints a blockchain
 * @blockchain: Pointer to the blockchain to print
 */
void _blockchain_print(blockchain_t const *blockchain)
{
    llist_t *current;
    block_t *block;
    int i, count = 0;
    
    if (!blockchain || !blockchain->chain)
        return;
        
    current = blockchain->chain;
    while (current)
    {
        if (current->node)
            count++;
        current = current->next;
    }
    
    printf("Blockchain: {\n");
    printf("    chain [%d]: [\n", count);
    
    current = blockchain->chain;
    while (current && current->node)
    {
        block = (block_t *)current->node;
        printf("        Block: {\n");
        printf("            info: {\n");
        printf("                index: %u,\n", block->info.index);
        printf("                difficulty: %u,\n", block->info.difficulty);
        printf("                timestamp: %lu,\n", block->info.timestamp);
        printf("                nonce: %lu,\n", block->info.nonce);
        printf("                prev_hash: ");
        for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
            printf("%02x", block->info.prev_hash[i]);
        printf("\n            },\n");
        printf("            data: {\n");
        printf("                buffer: \"%s\",\n", block->data.buffer);
        printf("                len: %u\n", block->data.len);
        printf("            },\n");
        printf("            hash: ");
        for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
            printf("%02x", block->hash[i]);
        printf("\n        }\n");
        current = current->next;
    }
    printf("    ]\n}\n");
}
