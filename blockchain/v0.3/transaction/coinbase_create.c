#include <stdlib.h>
#include <string.h>
#include "transaction.h"

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
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	uint8_t receiver_pub[EC_PUB_LEN];

	if (!receiver)
		return (NULL);

	/* Extract receiver's public key */
	if (!ec_to_pub(receiver, receiver_pub))
		return (NULL);

	/* Create transaction structure */
	transaction = calloc(1, sizeof(transaction_t));
	if (!transaction)
		return (NULL);

	/* Create inputs list */
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->inputs)
	{
		free(transaction);
		return (NULL);
	}

	/* Create outputs list */
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->outputs)
	{
		llist_destroy(transaction->inputs, 1, NULL);
		free(transaction);
		return (NULL);
	}

	/* Create transaction input (zeroed) */
	tx_in = calloc(1, sizeof(tx_in_t));
	if (!tx_in)
	{
		llist_destroy(transaction->inputs, 1, NULL);
		llist_destroy(transaction->outputs, 1, NULL);
		free(transaction);
		return (NULL);
	}

	/* Set block_index in the first 4 bytes of tx_out_hash to make it unique */
	memcpy(tx_in->tx_out_hash, &block_index, sizeof(block_index));

	/* Add input to transaction */
	if (llist_add_node(transaction->inputs, tx_in, ADD_NODE_REAR) != 0)
	{
		free(tx_in);
		llist_destroy(transaction->inputs, 1, NULL);
		llist_destroy(transaction->outputs, 1, NULL);
		free(transaction);
		return (NULL);
	}

	/* Create transaction output */
	tx_out = tx_out_create(COINBASE_AMOUNT, receiver_pub);
	if (!tx_out)
	{
		llist_destroy(transaction->inputs, 1, free);
		llist_destroy(transaction->outputs, 1, NULL);
		free(transaction);
		return (NULL);
	}

	/* Add output to transaction */
	if (llist_add_node(transaction->outputs, tx_out, ADD_NODE_REAR) != 0)
	{
		free(tx_out);
		llist_destroy(transaction->inputs, 1, free);
		llist_destroy(transaction->outputs, 1, NULL);
		free(transaction);
		return (NULL);
	}

	/* Compute transaction hash (ID) */
	if (!transaction_hash(transaction, transaction->id))
	{
		llist_destroy(transaction->inputs, 1, free);
		llist_destroy(transaction->outputs, 1, free);
		free(transaction);
		return (NULL);
	}

	return (transaction);
}

