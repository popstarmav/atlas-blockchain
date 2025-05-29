#include "hblk_crypto.h"
#include <openssl/pem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PRI_FILENAME "key.pem"
#define PUB_FILENAME "key_pub.pem"

/**
 * _alloc_key_paths - Allocates and builds file paths for keys
 * @folder: The folder path
 * @pri_path: Pointer to store private key path
 * @pub_path: Pointer to store public key path
 *
 * Return: 1 on success, 0 on failure
 */
static int _alloc_key_paths(char const *folder, char **pri_path, char **pub_path)
{
	size_t folder_len = strlen(folder);
	size_t pub_path_len = folder_len + 1 + strlen(PUB_FILENAME) + 1;
	size_t pri_path_len = folder_len + 1 + strlen(PRI_FILENAME) + 1;

	*pub_path = calloc(1, pub_path_len);
	if (!*pub_path)
		return (0);
	strcpy(*pub_path, folder);
	strcat(*pub_path, "/");
	strcat(*pub_path, PUB_FILENAME);

	*pri_path = calloc(1, pri_path_len);
	if (!*pri_path)
	{
		free(*pub_path);
		*pub_path = NULL;
		return (0);
	}
	strcpy(*pri_path, folder);
	strcat(*pri_path, "/");
	strcat(*pri_path, PRI_FILENAME);

	return (1);
}

/**
 * ec_save - Saves an existing EC key pair to disk
 * @key: Points to the EC key pair to be saved
 * @folder: Path to the folder in which to save the keys
 *
 * Return: 1 on success, 0 on failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *fp;
	char *pri_path = NULL, *pub_path = NULL;
	struct stat st;

	if (!key || !folder)
		return (0);

	if (mkdir(folder, 0700) == -1)
	{
		if (stat(folder, &st) != 0 || !S_ISDIR(st.st_mode))
			return (0);
	}

	if (!_alloc_key_paths(folder, &pri_path, &pub_path))
		return (0);

	fp = fopen(pri_path, "w");
	if (!fp)
	{
		free(pri_path);
		free(pub_path);
		return (0);
	}

	if (!PEM_write_ECPrivateKey(fp, key, NULL, NULL, 0, NULL, NULL))
	{
		fclose(fp);
		free(pri_path);
		free(pub_path);
		return (0);
	}
	fclose(fp);

	fp = fopen(pub_path, "w");
	if (!fp)
	{
		free(pri_path);
		free(pub_path);
		return (0);
	}

	if (!PEM_write_EC_PUBKEY(fp, key))
	{
		fclose(fp);
		free(pri_path);
		free(pub_path);
		return (0);
	}
	fclose(fp);

	free(pri_path);
	free(pub_path);
	return (1);
}

