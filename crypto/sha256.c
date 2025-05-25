#include "hblk_crypto.h"

/**
 * sha256 - Computes the hash of a sequence of bytes
 * @s: The sequence of bytes to be hashed
 * @len: The number of bytes to hash in s
 * @digest: The buffer to store the resulting hash
 *
 * Return: A pointer to digest, or NULL if digest is NULL
 */
uint8_t *sha256(int8_t const *s,
		size_t len, uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!digest)
		return (NULL);

	SHA256((unsigned char const *)s, len, digest);

	return (digest);
}
