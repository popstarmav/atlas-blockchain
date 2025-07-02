#include <stdio.h>
#include <string.h>
#include "../blockchain.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _tx_out_print - Prints a transaction output
 * @out: Pointer to the transaction output to print
 * @idx: Index (unused)
 * @indent: String to print before each line
 *
 * Return: 0
 */
static int _tx_out_print(tx_out_t const *out, unsigned int idx,
    char const *indent)
{
    if (!out)
    {
        printf("%s(null)\n", indent);
        return (0);
    }

    printf("%s{\n", indent);
    printf("%s\tamount: %u,\n", indent, out->amount);
    printf("%s\tpub: ", indent);
    _print_hex_buffer(out->pub, EC_PUB_LEN);
    printf(",\n");
    printf("%s\thash: ", indent);
    _print_hex_buffer(out->hash, SHA256_DIGEST_LENGTH);
    printf("\n%s}\n", indent);

    (void)idx;
    return (0);
}

/**
 * _tx_in_print - Prints a transaction input
 * @in: Pointer to the transaction input to print
 * @idx: Index (unused)
 * @indent: String to print before each line
 *
 * Return: 0
 */
static int _tx_in_print(tx_in_t const *in, unsigned int idx,
    char const *indent)
{
    if (!in)
    {
        printf("%s(null)\n", indent);
        return (0);
    }

    printf("%s{\n", indent);
    printf("%s\tblock_hash: ", indent);
    _print_hex_buffer(in->block_hash, SHA256_DIGEST_LENGTH);
    printf(",\n");
    printf("%s\ttx_id: ", indent);
    _print_hex_buffer(in->tx_id, SHA256_DIGEST_LENGTH);
    printf(",\n");
    printf("%s\ttx_out_hash: ", indent);
    _print_hex_buffer(in->tx_out_hash, SHA256_DIGEST_LENGTH);
    printf(",\n");
    printf("%s\tsig: {\n", indent);
    printf("%s\t\tsig: ", indent);
    _print_hex_buffer(in->sig.sig, in->sig.len);
    printf(",\n");
    printf("%s\t\tlen: %u\n", indent, in->sig.len);
    printf("%s\t}\n", indent);
    printf("%s}\n", indent);

    (void)idx;
    return (0);
}

/**
 * _transaction_print_loop - Prints a transaction (for use with llist_for_each)
 * @transaction: Pointer to the transaction to print
 * @idx: Index (unused)
 * @indent: String to print before each line
 *
 * Return: 0
 */
int _transaction_print_loop(transaction_t const *transaction,
    unsigned int idx, char const *indent)
{
    char new_indent[256];

    if (!transaction)
    {
        printf("%s(null)\n", indent);
        return (0);
    }

    sprintf(new_indent, "%s\t", indent);

    printf("%s{\n", indent);
    printf("%s\tid: ", indent);
    _print_hex_buffer(transaction->id, SHA256_DIGEST_LENGTH);
    printf(",\n");
    printf("%s\tinputs [%d]: [\n", indent, llist_size(transaction->inputs));
    llist_for_each(transaction->inputs, (node_func_t)_tx_in_print, new_indent);
    printf("%s\t],\n", indent);
    printf("%s\toutputs [%d]: [\n", indent, llist_size(transaction->outputs));
    llist_for_each(transaction->outputs, (node_func_t)_tx_out_print, new_indent);
    printf("%s\t]\n", indent);
    printf("%s}\n", indent);

    (void)idx;
    return (0);
}

/**
 * _transaction_print_brief_loop - Prints a transaction briefly
 * @transaction: Pointer to the transaction to print
 * @idx: Index (unused)
 * @indent: String to print before each line
 *
 * Return: 0
 */
int _transaction_print_brief_loop(transaction_t const *transaction,
    unsigned int idx, char const *indent)
{
    if (!transaction)
    {
        printf("%s(null)\n", indent);
        return (0);
    }

    printf("%s{ ", indent);
    _print_hex_buffer(transaction->id, SHA256_DIGEST_LENGTH);
    printf(", [%d], [%d] }\n", 
           llist_size(transaction->inputs), 
           llist_size(transaction->outputs));

    (void)idx;
    return (0);
}

/**
 * _transaction_print_brief - Prints a transaction briefly
 * @transaction: Pointer to the transaction to print
 */
void _transaction_print_brief(transaction_t const *transaction)
{
    if (!transaction)
    {
        printf("(null)\n");
        return;
    }

    printf("Transaction: { ");
    _print_hex_buffer(transaction->id, SHA256_DIGEST_LENGTH);
    printf(", [%d], [%d] }\n", 
           llist_size(transaction->inputs), 
           llist_size(transaction->outputs));
}

/**
 * _transaction_print - Prints a transaction
 * @transaction: Pointer to the transaction to print
 */
void _transaction_print(transaction_t const *transaction)
{
    if (!transaction)
    {
        printf("Transaction: (null)\n");
        return;
    }

    printf("Transaction: {\n");
    printf("\tid: ");
    _print_hex_buffer(transaction->id, SHA256_DIGEST_LENGTH);
    printf(",\n");
    printf("\tinputs [%d]: [\n", llist_size(transaction->inputs));
    llist_for_each(transaction->inputs, (node_func_t)_tx_in_print, "\t\t");
    printf("\t],\n");
    printf("\toutputs [%d]: [\n", llist_size(transaction->outputs));
    llist_for_each(transaction->outputs, (node_func_t)_tx_out_print, "\t\t");
    printf("\t]\n");
    printf("}\n");
}

