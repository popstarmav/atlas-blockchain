#include "hblk_crypto.h"
#include <openssl/pem.h>
#include <stdio.h>
#include <string.h>

/**
 * ec_load - Loads an EC key pair from the disk
 * @folder: Path to the folder from which to load the keys
 *
 * Return: Pointer to the created EC key pair, or NULL on failure
 */
EC_KEY *ec_load(char const *folder)
{
	FILE *fp;
	char path[1024];
	EC_KEY *key = NULL;

	if (!folder)
		return (NULL);

	snprintf(path, sizeof(path), "%s/key.pem", folder);
	fp = fopen(path, "r");
	if (!fp)
		return (NULL);

	key = PEM_read_ECPrivateKey(fp, NULL, NULL, NULL);
	fclose(fp);

	if (!key)
		return (NULL);

	return (key);
}

