#include <stdio.h>
#include <string.h>
#include "blockchain.h"

/**
 * _print_hex_buffer - Prints a buffer in its hexadecimal form
 * @buf: Pointer to the buffer to be printed
 * @len: Number of bytes from @buf to be printed
 */
static void _print_hex_buffer(uint8_t const *buf, size_t len)
{
    size_t i;
    for (i = 0; buf && i < len; i++)
        printf("%02x", buf[i]);
    fflush(NULL);
}

/**
 * _block_print - Prints information about a Block
 * @block: Pointer to the Block to be printed
 * @index: Index of the Block in the Blockchain
 * @indent: Lines prefix
 * Return: 0
 */
static int _block_print(block_t const *block, unsigned int index,
    char const *indent)
{
    if (!block)
    {
        printf("%snil\n", indent);
        return (0);
    }
    
    printf("%sBlock: {\n", indent);
    printf("%s\tinfo: {\n", indent);
    printf("%s\t\tindex: %u,\n", indent, block->info.index);
    printf("%s\t\tdifficulty: %u,\n", indent, block->info.difficulty);
    printf("%s\t\ttimestamp: %lu,\n", indent, block->info.timestamp);
    printf("%s\t\tnonce: %lu,\n", indent, block->info.nonce);
    printf("%s\t\tprev_hash: ", indent);
    _print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
    printf("\n%s\t},\n", indent);
    printf("%s\tdata: {\n", indent);
    printf("%s\t\tbuffer: \"%s\",\n", indent, block->data.buffer);
    printf("%s\t\tlen: %u\n", indent, block->data.len);
    printf("%s\t},\n", indent);
    printf("%s\thash: ", indent);
    _print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
    printf("\n%s}\n", indent);
    
    (void)index;
    return (0);
}

/**
 * _blockchain_print - Prints an entire Blockchain
 * @blockchain: Pointer to the Blockchain to be printed
 */
void _blockchain_print(blockchain_t const *blockchain)
{
    printf("Blockchain: {\n");
    printf("\tchain [%d]: [\n", llist_size(blockchain->chain));
    llist_for_each(blockchain->chain, (node_func_t)_block_print, "\t\t");
    printf("\t]\n");
    printf("}\n");
    fflush(NULL);
}

