#include <stdlib.h>
#include "transaction.h"

void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    transaction_t *coinbase;
    EC_KEY *owner;

    owner = ec_create();
    coinbase = coinbase_create(owner, 1);
    _transaction_print(coinbase);

    if (coinbase_is_valid(coinbase, 1))
        printf("Coinbase transaction valid\n");
    else
        fprintf(stderr, "Coinbase transaction invalid\n");

    /* Cleanup */
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    EC_KEY_free(owner);
    #pragma GCC diagnostic pop

    return (EXIT_SUCCESS);
}

