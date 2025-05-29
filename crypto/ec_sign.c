#include "hblk_crypto.h"
#include <openssl/sha.h>
#include <openssl/ecdsa.h>
#include <openssl/err.h>
#include <string.h>

/**
 * ec_sign - Signs a given set of bytes using EC_KEY private key
 * @key: Points to the EC_KEY structure containing the private key
 * @msg: Points to the msglen characters to be signed
 * @msglen: Length of the message
 * @sig: Holds the address at which to store the signature
 *
 * Return: Pointer to the signature buffer upon success, NULL upon failure
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t *sig)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];
	unsigned int sig_len;

	if (!key || !msg || !sig)
		return (NULL);

	if (!EC_KEY_check_key(key))
		return (NULL);

	if (!SHA256(msg, msglen, hash))
		return (NULL);

	memset(sig, 0, sizeof(sig_t));
	sig_len = 0;

	if (!ECDSA_sign(0, hash, SHA256_DIGEST_LENGTH, sig->sig, &sig_len, (EC_KEY *)key))
		return (NULL);

	if (sig_len > SIG_MAX_LEN)
		return (NULL);

	sig->len = sig_len;

	return (sig->sig);
}
