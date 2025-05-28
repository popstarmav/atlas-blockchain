#include "hblk_crypto.h"

/**
 * cleanup_and_return_null - Helper function to clean up resources
 * @key: EC_KEY to free (can be NULL)
 * @group: EC_GROUP to free (can be NULL)
 * @point: EC_POINT to free (can be NULL)
 *
 * Return: Always returns NULL
 */
static EC_KEY *cleanup_and_return_null(EC_KEY *key, EC_GROUP *group,
				       EC_POINT *point)
{
	if (point)
		EC_POINT_free(point);
	if (group)
		EC_GROUP_free(group);
	if (key)
		EC_KEY_free(key);
	return (NULL);
}

/**
 * ec_from_pub - Creates an EC_KEY structure from a public key
 * @pub: Contains the public key to be converted (uncompressed format)
 *
 * Return: Pointer to the created EC_KEY structure on success,
 *         NULL on failure
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key = NULL;
	EC_GROUP *group = NULL;
	EC_POINT *pub_point = NULL;

	if (pub == NULL)
		return (NULL);

	key = EC_KEY_new();
	if (key == NULL)
		return (NULL);

	group = EC_GROUP_new_by_curve_name(NID_secp256k1);
	if (group == NULL)
		return (cleanup_and_return_null(key, NULL, NULL));

	if (EC_KEY_set_group(key, group) != 1)
		return (cleanup_and_return_null(key, group, NULL));

	pub_point = EC_POINT_new(group);
	if (pub_point == NULL)
		return (cleanup_and_return_null(key, group, NULL));

	if (EC_POINT_oct2point(group, pub_point, pub, EC_PUB_LEN, NULL) != 1)
		return (cleanup_and_return_null(key, group, pub_point));

	if (EC_KEY_set_public_key(key, pub_point) != 1)
		return (cleanup_and_return_null(key, group, pub_point));

	EC_POINT_free(pub_point);
	EC_GROUP_free(group);
	return (key);
}

