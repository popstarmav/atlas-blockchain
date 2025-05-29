#include "hblk_crypto.h"
#include <openssl/sha.h>
#include <openssl/ecdsa.h>

/**
 * ec_verify - Verifies the signature of a given set of bytes
 * @key: Points to the EC_KEY structure containing the public key
 * @msg: Points to the msglen characters to verify the signature of
 * @msglen: Length of the message
 * @sig: Points to the signature to be checked
 *
 * Return: 1 if the signature is valid, 0 otherwise
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t const *sig)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];

	if (!key || !msg || !sig)
		return (0);

	if (!SHA256(msg, msglen, hash))
		return (0);

	return (ECDSA_verify(0, hash, SHA256_DIGEST_LENGTH, sig->sig, sig->len, (EC_KEY *)key));
}

