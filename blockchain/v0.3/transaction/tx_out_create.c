#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * tx_out_create - Creates a transaction output
 * @amount: Amount of the transaction output
 * @pub: Public key of the receiver
 *
 * Return: Pointer to the created transaction output, or NULL on failure
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tx_out;

	if (!pub)
		return (NULL);

	tx_out = malloc(sizeof(tx_out_t));

	if (!tx_out)
		return (NULL);

	tx_out->amount = amount;
	memcpy(tx_out->pub, pub, EC_PUB_LEN);

	/* Compute hash of the transaction output */
	if (!sha256((int8_t const *)tx_out,
				sizeof(tx_out->amount) +
	EC_PUB_LEN, tx_out->hash))
	{
		free(tx_out);
		return (NULL);
	}

	return (tx_out);
}
