#include <string.h>
#include "transaction.h"

/**
 * find_unspent_output - Finds an unspent output matching the given hash
 * @all_unspent: List of all unspent transaction outputs
 * @tx_out_hash: Hash of the transaction output to find
 *
 * Return: Pointer to the unspent output if found, NULL otherwise
 */
static unspent_tx_out_t *find_unspent_output(llist_t *all_unspent, 
                                            uint8_t const *tx_out_hash)
{
    unspent_tx_out_t *unspent;
    int i, size;

    size = llist_size(all_unspent);
    for (i = 0; i < size; i++)
    {
        unspent = llist_get_node_at(all_unspent, i);
        if (!unspent)
            continue;
        
        if (memcmp(unspent->out.hash, tx_out_hash, SHA256_DIGEST_LENGTH) == 0)
            return (unspent);
    }
    
    return (NULL);
}

/**
 * check_inputs - Validates all transaction inputs
 * @transaction: The transaction to validate
 * @all_unspent: List of all unspent transaction outputs
 * @total_input: Pointer to store the total input amount
 *
 * Return: 1 if all inputs are valid, 0 otherwise
 */
static int check_inputs(transaction_t const *transaction, 
                       llist_t *all_unspent, 
                       uint64_t *total_input)
{
    tx_in_t *input;
    unspent_tx_out_t *unspent;
    EC_KEY *key;
    int i, size, valid;
    
    *total_input = 0;
    size = llist_size(transaction->inputs);
    
    for (i = 0; i < size; i++)
    {
        input = llist_get_node_at(transaction->inputs, i);
        if (!input)
            return (0);
            
        /* Find corresponding unspent output */
        unspent = find_unspent_output(all_unspent, input->tx_out_hash);
        if (!unspent)
            return (0);
            
        /* Verify signature */
        key = ec_from_pub(unspent->out.pub);
        if (!key)
            return (0);
            
        valid = ec_verify(key, transaction->id, SHA256_DIGEST_LENGTH, &input->sig);
        
        /* Suppress deprecated function warning */
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        EC_KEY_free(key);
        #pragma GCC diagnostic pop
        
        if (!valid)
            return (0);
            
        /* Add to total input amount */
        *total_input += unspent->out.amount;
    }
    
    return (1);
}

/**
 * calculate_outputs_total - Calculates the total amount of all outputs
 * @outputs: List of transaction outputs
 *
 * Return: Total amount of all outputs
 */
static uint64_t calculate_outputs_total(llist_t *outputs)
{
    tx_out_t *output;
    uint64_t total = 0;
    int i, size;
    
    size = llist_size(outputs);
    for (i = 0; i < size; i++)
    {
        output = llist_get_node_at(outputs, i);
        if (output)
            total += output->amount;
    }
    
    return (total);
}

/**
 * transaction_is_valid - Checks if a transaction is valid
 * @transaction: Points to the transaction to verify
 * @all_unspent: List of all unspent transaction outputs to date
 *
 * Return: 1 if the transaction is valid, 0 otherwise
 */
int transaction_is_valid(transaction_t const *transaction,
                        llist_t *all_unspent)
{
    uint8_t computed_hash[SHA256_DIGEST_LENGTH];
    uint64_t total_input = 0, total_output = 0;
    
    if (!transaction || !all_unspent)
        return (0);
        
    /* Verify transaction hash */
    if (!transaction_hash(transaction, computed_hash) ||
        memcmp(computed_hash, transaction->id, SHA256_DIGEST_LENGTH) != 0)
        return (0);
        
    /* Validate all inputs and calculate total input amount */
    if (!check_inputs(transaction, all_unspent, &total_input))
        return (0);
        
    /* Calculate total output amount */
    total_output = calculate_outputs_total(transaction->outputs);
    
    /* Verify that input total equals output total */
    return (total_input == total_output);
}

