#include "../blockchain.h"

/**
 * _block_print_brief - Prints a Block in a brief format
 * @block: Pointer to the Block to print
 * @index: Unused parameter
 * @indent: String to print before each line
 *
 * Return: 0
 */
static int _block_print_brief(block_t const *block, unsigned int index, char const *indent)
{
    unsigned int i;
    
    (void)index;
    
    printf("%sBlock: {\n", indent);
    printf("%s\tinfo: { %u, %u, %lu, %lu, ", indent,
           block->info.index, block->info.difficulty,
           block->info.timestamp, block->info.nonce);
    
    /* Print prev_hash */
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        printf("%02x", block->info.prev_hash[i]);
    printf(" },\n");
    
    printf("%s\tdata: { \"%s\", %u },\n", indent,
           (char *)block->data.buffer, block->data.len);
    
    printf("%s\thash: ", indent);
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        printf("%02x", block->hash[i]);
    printf("\n%s}\n", indent);
    
    return (0);
}

/**
 * _blockchain_print_brief - Prints a Blockchain in a brief format
 * @blockchain: Pointer to the Blockchain to print
 */
void _blockchain_print_brief(blockchain_t const *blockchain)
{
    printf("Blockchain: {\n");
    printf("\tchain [%d]: [\n", llist_size(blockchain->chain));
    llist_for_each(blockchain->chain, (node_func_t)_block_print_brief, "\t\t");
    printf("\t]\n");
    printf("}\n");
    fflush(NULL);
}

