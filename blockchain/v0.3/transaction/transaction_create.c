#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * struct tx_data_s - Helper structure for transaction creation
 * @pub: Sender's public key
 * @needed: Amount needed for the transaction
 * @amount_total: Total amount collected from UTXOs
 * @txt: Pointer to the transaction being built
 * @sender: Sender's private key
 * @all_unspent: List of all unspent outputs
 */
typedef struct tx_data_s
{
    uint8_t pub[EC_PUB_LEN];
    uint32_t needed;
    uint32_t amount_total;
    transaction_t *txt;
    EC_KEY const *sender;
    llist_t *all_unspent;
} tx_data_t;

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
    tx_out_t *out;
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

    /* Initialize data structure */
    if (!ec_to_pub(sender, data->pub) || !ec_to_pub(receiver, receiver_pub))
    {
        free(transaction);
        free(data);
        return (NULL);
    }

    data->needed = amount;
    data->txt = transaction;
    data->sender = sender;
    data->all_unspent = all_unspent;
    data->amount_total = 0;

    /* Create input and output lists */
    transaction->inputs = llist_create(MT_SUPPORT_FALSE);
    transaction->outputs = llist_create(MT_SUPPORT_FALSE);
    if (!transaction->inputs || !transaction->outputs)
    {
        if (transaction->inputs)
            llist_destroy(transaction->inputs, 0, NULL);
        if (transaction->outputs)
            llist_destroy(transaction->outputs, 0, NULL);
        free(transaction);
        free(data);
        return (NULL);
    }

    /* Collect UTXOs belonging to sender */
    llist_for_each(all_unspent, build_transaction, data);

    /* Check if we have enough funds */
    if (data->amount_total < data->needed)
    {
        llist_destroy(transaction->inputs, 1, free);
        llist_destroy(transaction->outputs, 1, free);
        free(transaction);
        free(data);
        return (NULL);
    }

    /* Create output for receiver */
    out = tx_out_create(data->needed, receiver_pub);
    if (!out)
    {
        llist_destroy(transaction->inputs, 1, free);
        llist_destroy(transaction->outputs, 1, free);
        free(transaction);
        free(data);
        return (NULL);
    }
    llist_add_node(transaction->outputs, out, ADD_NODE_REAR);

    /* Create change output if necessary */
    if (data->amount_total != data->needed)
    {
        out = tx_out_create(data->amount_total - data->needed, data->pub);
        if (!out)
        {
            llist_destroy(transaction->inputs, 1, free);
            llist_destroy(transaction->outputs, 1, free);
            free(transaction);
            free(data);
            return (NULL);
        }
        llist_add_node(transaction->outputs, out, ADD_NODE_REAR);
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

