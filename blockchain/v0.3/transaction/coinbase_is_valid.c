#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * check_hash_validity - Verifies transaction hash
 * @coinbase: Points to the coinbase transaction to verify
 *
 * Return: 1 if hash is valid, 0 otherwise
 */
static int check_hash_validity(transaction_t const *coinbase)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];

	if (!transaction_hash(coinbase, hash))
		return (0);

	if (memcmp(hash, coinbase->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	return (1);
}

/**
 * check_input_validity - Verifies input validity
 * @tx_in: Transaction input to verify
 * @block_index: Index of the Block the coinbase transaction will belong to
 *
 * Return: 1 if input is valid, 0 otherwise
 */
static int check_input_validity(tx_in_t const *tx_in, uint32_t block_index)
{
	uint8_t zero_hash[SHA256_DIGEST_LENGTH] = {0};
	uint8_t block_index_bytes[4];

	/* Convert block_index to bytes (little-endian) */
	block_index_bytes[0] = block_index & 0xFF;
	block_index_bytes[1] = (block_index >> 8) & 0xFF;
	block_index_bytes[2] = (block_index >> 16) & 0xFF;
	block_index_bytes[3] = (block_index >> 24) & 0xFF;

	/* Verify first 4 bytes of tx_out_hash match block_index */
	if (memcmp(tx_in->tx_out_hash, block_index_bytes, 4) != 0)
		return (0);

	/* Verify block_hash, tx_id, and signature are zeroed */
	if (memcmp(tx_in->block_hash, zero_hash, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (memcmp(tx_in->tx_id, zero_hash, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	/* Check that signature length is 0 */
	if (tx_in->sig.len != 0)
		return (0);

	return (1);
}

/**
 * coinbase_is_valid - Checks if a coinbase transaction is valid
 * @coinbase: Points to the coinbase transaction to verify
 * @block_index: Index of the Block the coinbase transaction will belong to
 *
 * Return: 1 if the coinbase transaction is valid, 0 otherwise
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	tx_in_t *tx_in;
	tx_out_t *tx_out;

	if (!coinbase)
		return (0);

	/* Verify transaction hash */
	if (!check_hash_validity(coinbase))
		return (0);

	/* Verify exactly 1 input and 1 output */
	if (llist_size(coinbase->inputs) != 1 || llist_size(coinbase->outputs) != 1)
		return (0);

	/* Get the input and output */
	tx_in = llist_get_node_at(coinbase->inputs, 0);
	tx_out = llist_get_node_at(coinbase->outputs, 0);
	if (!tx_in || !tx_out)
		return (0);

	/* Verify input validity */
	if (!check_input_validity(tx_in, block_index))
		return (0);

	/* Verify output amount is exactly COINBASE_AMOUNT */
	if (tx_out->amount != COINBASE_AMOUNT)
		return (0);

	return (1);
}

