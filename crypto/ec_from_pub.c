#include "hblk_crypto.h"

/**
 * ec_from_pub - Creates an EC_KEY structure from a public key
 * @pub: Contains the public key to be converted (uncompressed format)
 *
 * Return: Pointer to the created EC_KEY structure on success, NULL on failure
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
	{
		EC_KEY_free(key);
		return (NULL);
	}
	
	if (EC_KEY_set_group(key, group) != 1)
	{
		EC_GROUP_free(group);
		EC_KEY_free(key);
		return (NULL);
	}
	
	pub_point = EC_POINT_new(group);
	
	if (pub_point == NULL)
	{
		EC_GROUP_free(group);
		EC_KEY_free(key);
		return (NULL);
	}
	
	if (EC_POINT_oct2point(group, pub_point, pub, EC_PUB_LEN, NULL) != 1)
	{
		EC_POINT_free(pub_point);
		EC_GROUP_free(group);
		EC_KEY_free(key);
		return (NULL);
	}
	
	/* SET the public key BEFORE freeing the point */
	if (EC_KEY_set_public_key(key, pub_point) != 1)
	{
		EC_POINT_free(pub_point);
		EC_GROUP_free(group);
		EC_KEY_free(key);
		return (NULL);
	}
	
	/* Now we can free the temporary objects */
	EC_POINT_free(pub_point);
	EC_GROUP_free(group);
	
	return (key);
}
