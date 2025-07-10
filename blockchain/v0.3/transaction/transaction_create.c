#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * build_transaction - builds the transaction inputs
 * @node: the node in the unspent list
 * @i: unused iter
 * @txt_data: transaction data struct
 *
 * Return: 0 to continue, 1 when done
 */
static int build_transaction(llist_node_t node, unsigned int i, void *txt_data)
{
	(void)i;
	unspent_tx_out_t *utxo = node;
	tx_data_t *data = txt_data;
	tx_in_t *in;

	if (data->needed <= data->amount_total || !utxo)
		return (1);

	if (memcmp(data->pub, utxo->out.pub, EC_PUB_LEN) == 0)
	{
		in = tx_in_create(utxo);
		if (!in)
			return (1);

		llist_add_node(data->txt->inputs, in, ADD_NODE_REAR);
		data->amount_total += utxo->out.amount;
	}

	return (0);
}

/**
 * sign_ins - signs all the inputs
 * @node: the node in the inputs list
 * @i: unused iter
 * @txt_data: transaction data struct
 *
 * Return: 0 to continue, 1 on error
 */
static int sign_ins(llist_node_t node, unsigned int i, void *txt_data)
{
	(void)i;
	tx_in_t *in = node;
	tx_data_t *data = txt_data;

	if (!in)
		return (1);

	if (!tx_in_sign(in, data->txt->id, data->sender, data->all_unspent))
		return (1);

	return (0);
}

/**
 * init_transaction_data - Initializes transaction data structure
 * @data: Pointer to data structure to initialize
 * @transaction: Pointer to transaction
 * @sender: Sender's private key
 * @receiver: Receiver's public key
 * @amount: Amount to send
 * @all_unspent: List of all unspent outputs
 *
 * Return: 1 on success, 0 on failure
 */
static int init_transaction_data(tx_data_t *data, transaction_t *transaction,
	EC_KEY const *sender, EC_KEY const *receiver,
	uint32_t amount, llist_t *all_unspent)
{
	uint8_t receiver_pub[EC_PUB_LEN];

	if (!ec_to_pub(sender, data->pub) || !ec_to_pub(receiver, receiver_pub))
		return (0);

	data->needed = amount;
	data->txt = transaction;
	data->sender = sender;
	data->all_unspent = all_unspent;
	data->amount_total = 0;

	/* Create input and output lists */
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->inputs || !transaction->outputs)
		return (0);

	return (1);
}

/**
 * create_transaction_outputs - Creates transaction outputs
 * @transaction: Pointer to transaction
 * @data: Transaction data
 * @receiver_pub: Receiver's public key
 *
 * Return: 1 on success, 0 on failure
 */
static int create_transaction_outputs(transaction_t *transaction,
	tx_data_t *data, uint8_t receiver_pub[EC_PUB_LEN])
{
	tx_out_t *out;

	/* Create output for receiver */
	out = tx_out_create(data->needed, receiver_pub);
	if (!out)
		return (0);

	if (llist_add_node(transaction->outputs, out, ADD_NODE_REAR) != 0)
	{
		free(out);
		return (0);
	}

	/* Create change output if necessary */
	if (data->amount_total != data->needed)
	{
		out = tx_out_create(data->amount_total - data->needed, data->pub);
		if (!out)
			return (0);

		if (llist_add_node(transaction->outputs, out, ADD_NODE_REAR) != 0)
		{
			free(out);
			return (0);
		}
	}

	return (1);
}

/**
 * transaction_create - Creates a transaction
 * @sender: Contains the private key of the transaction sender
 * @receiver: Contains the public key of the transaction receiver
 * @amount: Amount to send
 * @all_unspent: List of all unspent outputs to date
 *
 * Return: Pointer to the created transaction upon success, or NULL upon failure
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
	uint32_t amount, llist_t *all_unspent)
{
	transaction_t *transaction;
	tx_data_t *data;
	uint8_t receiver_pub[EC_PUB_LEN];

	if (!sender || !receiver || !amount || !all_unspent)
		return (NULL);

	transaction = calloc(1, sizeof(transaction_t));
	if (!transaction)
		return (NULL);

	data = calloc(1, sizeof(tx_data_t));
	if (!data)
	{
		free(transaction);
		return (NULL);
	}

	if (!ec_to_pub(receiver, receiver_pub) ||
		!init_transaction_data(data, transaction, sender, receiver, amount, all_unspent))
	{
		free(transaction);
		free(data);
		return (NULL);
	}

	/* Collect UTXOs belonging to sender */
	llist_for_each(all_unspent, build_transaction, data);

	/* Check if we have enough funds */
	if (data->amount_total < data->needed ||
		!create_transaction_outputs(transaction, data, receiver_pub))
	{
		llist_destroy(transaction->inputs, 1, free);
		llist_destroy(transaction->outputs, 1, free);
		free(transaction);
		free(data);
		return (NULL);
	}

	/* Compute transaction hash */
	if (!transaction_hash(transaction, transaction->id))
	{
		llist_destroy(transaction->inputs, 1, free);
		llist_destroy(transaction->outputs, 1, free);
		free(transaction);
		free(data);
		return (NULL);
	}

	/* Sign all inputs */
	llist_for_each(transaction->inputs, sign_ins, data);
	free(data);

	return (transaction);
}

