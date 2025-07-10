#include <stdlib.h>
#include "transaction.h"

/**
 * transaction_destroy - Deallocates a transaction structure
 * @transaction: Points to the transaction to delete
 */
void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;

	/* Free all inputs */
	if (transaction->inputs)
	{
		llist_destroy(transaction->inputs, 1, free);
	}

	/* Free all outputs */
	if (transaction->outputs)
	{
		llist_destroy(transaction->outputs, 1, free);
	}

	/* Free the transaction itself */
	free(transaction);
}

