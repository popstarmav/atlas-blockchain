#include "hblk_crypto.h"
#include <openssl/pem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

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
	char path[1024];
	struct stat st;

	if (!key || !folder)
		return (0);

	if (mkdir(folder, 0700) == -1)
	{
		if (stat(folder, &st) != 0 || !S_ISDIR(st.st_mode))
			return (0);
	}

	snprintf(path, sizeof(path), "%s/key.pem", folder);
	fp = fopen(path, "w");
	if (!fp)
		return (0);

	if (!PEM_write_ECPrivateKey(fp, key, NULL, NULL, 0, NULL, NULL))
	{
		fclose(fp);
		return (0);
	}
	fclose(fp);

	snprintf(path, sizeof(path), "%s/key_pub.pem", folder);
	fp = fopen(path, "w");
	if (!fp)
		return (0);

	if (!PEM_write_EC_PUBKEY(fp, key))
	{
		fclose(fp);
		return (0);
	}
	fclose(fp);

	return (1);
}

