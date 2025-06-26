#include "../blockchain.h"

/** 
 * _print_hex_buffer - Prints a buffer in hexadecimal format
 * @buf: Pointer to the buffer to be printed
 * @len: Number of bytes from @buf to be printed
 */
static void _print_hex_buffer(uint8_t const *buf, size_t len)
{
    size_t i;
    
    for (i = 0; buf && i < len; i++)
        printf("%02x", buf[i]);
}

/**
 * _block_print_brief - Prints information about a Block (brief)
 * @block: Pointer to the Block to be printed
 * @index: Index of the Block in the Blockchain  
 * @indent: String to be printed before each line
 *
 * Return: 0
 */
static int _block_print_brief(block_t const *block, unsigned int index, char const *indent)
{
    if (!block)
    {
        printf("%sBlock->NULL\n", indent);
        return (1);
    }
    
    printf("%sBlock: {\n", indent);
    printf("%s\tinfo: { %u, %u, %lu, %lu, ", indent, 
           block->info.index, block->info.difficulty,
           block->info.timestamp, block->info.nonce);
    _print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
    printf(" },\n");
    printf("%s\tdata: { \"%s\", %u },\n", indent, 
           (char const *)block->data.buffer, block->data.len);
    printf("%s\thash: ", indent);
    _print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
    printf("\n%s}\n", indent);
    
    (void)index;
    return (0);
}

/**
 * _blockchain_print_brief - Prints an entire Blockchain (brief)
 * @blockchain: Pointer to the Blockchain to be printed
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

