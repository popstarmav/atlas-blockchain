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

		llist_add_node(data->txt->inputs, in, 
				ADD_NODE_REAR);
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

	if (!tx_in_sign(in, data->txt->id,
				data->sender, data->all_unspent))
		return (1);

	return (0);
}

/**
 * create_transaction_lists - Creates input and output lists
 * @transaction: Transaction to initialize
 *
 * Return: 1 on success, 0 on failure
 */
static int create_transaction_lists(transaction_t *transaction)
{
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);

	if (!transaction->inputs || !transaction->outputs)
		return (0);

	return (1);
}

/**
 * cleanup_transaction - Cleans up transaction resources
 * @transaction: Transaction to clean up
 */
static void cleanup_transaction(transaction_t *transaction)
{
	if (transaction->inputs)
		llist_destroy(transaction->inputs, 1, free);
	if (transaction->outputs)
		llist_destroy(transaction->outputs, 1, free);
	free(transaction);
}

/**
 * create_receiver_output - Creates output for receiver
 * @transaction: Transaction to add output to
 * @amount: Amount to send
 * @receiver_pub: Receiver's public key
 *
 * Return: 1 on success, 0 on failure
 */
static int create_receiver_output(transaction_t *transaction,
	uint32_t amount, uint8_t receiver_pub[EC_PUB_LEN])
{
	tx_out_t *out;

	out = tx_out_create(amount, receiver_pub);
	if (!out)
		return (0);

	if (llist_add_node(transaction->outputs,
				out, ADD_NODE_REAR) != 0)
	{
		free(out);
		return (0);
	}

	return (1);
}

/**
 * create_change_output - Creates change output if necessary
 * @transaction: Transaction to add output to
 * @data: Transaction data
 *
 * Return: 1 on success, 0 on failure
 */
static int create_change_output(transaction_t *transaction, tx_data_t *data)
{
	tx_out_t *out;

	if (data->amount_total <= data->needed)
		return (1);

	out = tx_out_create(data->amount_total - data->needed, data->pub);
	if (!out)
		return (0);

	if (llist_add_node(transaction->outputs, out, ADD_NODE_REAR) != 0)
	{
		free(out);
		return (0);
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
	tx_data_t data;
	uint8_t receiver_pub[EC_PUB_LEN];

	if (!sender || !receiver || !amount || !all_unspent ||
		!ec_to_pub(sender, data.pub) ||
		!ec_to_pub(receiver, receiver_pub))
		return (NULL);

	transaction = calloc(1, sizeof(transaction_t));
	if (!transaction || !create_transaction_lists(transaction))
	{
		free(transaction);
		return (NULL);
	}

	/* Initialize data and collect UTXOs */
	data.needed = amount;
	data.txt = transaction;
	data.sender = sender;
	data.all_unspent = all_unspent;
	data.amount_total = 0;
	llist_for_each(all_unspent, build_transaction, &data);

	/* Check funds and create outputs */
	if (data.amount_total < data.needed ||
		!create_receiver_output(transaction, amount, receiver_pub) ||
		!create_change_output(transaction, &data) ||
		!transaction_hash(transaction, transaction->id))
	{
		cleanup_transaction(transaction);
		return (NULL);
	}

	/* Sign inputs and return */
	llist_for_each(transaction->inputs, sign_ins, &data);
	return (transaction);
}

