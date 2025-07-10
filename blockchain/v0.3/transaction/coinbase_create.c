#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * create_transaction_lists - Creates input and output lists for a transaction
 * @transaction: Transaction to initialize
 *
 * Return: 1 on success, 0 on failure
 */
static int create_transaction_lists(transaction_t *transaction)
{
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->inputs)
		return (0);

	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->outputs)
	{
		llist_destroy(transaction->inputs, 1, NULL);
		return (0);
	}

	return (1);
}

/**
 * create_coinbase_input - Creates a coinbase transaction input
 * @block_index: Index of the Block the coinbase transaction will belong to
 *
 * Return: Pointer to the created input, or NULL on failure
 */
static tx_in_t *create_coinbase_input(uint32_t block_index)
{
	tx_in_t *tx_in;

	tx_in = calloc(1, sizeof(tx_in_t));
	if (!tx_in)
		return (NULL);

	/* Set block_index in the first 4 bytes of tx_out_hash to make it unique */
	memcpy(tx_in->tx_out_hash, &block_index, sizeof(block_index));

	return (tx_in);
}

/**
 * cleanup_transaction - Frees all resources of a transaction
 * @transaction: Transaction to clean up
 * @tx_in: Transaction input to free if not NULL
 * @tx_out: Transaction output to free if not NULL
 */
static void cleanup_transaction(
	transaction_t *transaction,
	tx_in_t *tx_in,
	tx_out_t *tx_out)
{
	if (tx_in)
		free(tx_in);
	if (tx_out)
		free(tx_out);
	if (transaction->inputs)
		llist_destroy(transaction->inputs, 1, free);
	if (transaction->outputs)
		llist_destroy(transaction->outputs, 1, free);
	free(transaction);
}

/**
 * coinbase_create - Creates a coinbase transaction
 * @receiver: Contains the public key of the miner who will receive the coins
 * @block_index: Index of the Block the coinbase transaction will belong to
 *
 * Return: Pointer to the created transaction, or NULL upon failure
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *transaction;
	tx_in_t *tx_in = NULL;
	tx_out_t *tx_out = NULL;
	uint8_t receiver_pub[EC_PUB_LEN];

	if (!receiver || !ec_to_pub(receiver, receiver_pub))
		return (NULL);

	transaction = calloc(1, sizeof(transaction_t));
	if (!transaction)
		return (NULL);

	if (!create_transaction_lists(transaction))
	{
		free(transaction);
		return (NULL);
	}

	tx_in = create_coinbase_input(block_index);
	if (!tx_in || llist_add_node(transaction->inputs, tx_in, ADD_NODE_REAR) != 0)
	{
		cleanup_transaction(transaction, tx_in, NULL);
		return (NULL);
	}

	tx_out = tx_out_create(COINBASE_AMOUNT, receiver_pub);
	if (!tx_out || llist_add_node(transaction->outputs,
				tx_out, ADD_NODE_REAR) != 0)
	{
		cleanup_transaction(transaction, NULL, tx_out);
		return (NULL);
	}

	if (!transaction_hash(transaction, transaction->id))
	{
		cleanup_transaction(transaction, NULL, NULL);
		return (NULL);
	}

	return (transaction);
}

