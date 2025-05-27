#include "hblk_crypto.h"

/**
 * ec_to_pub - Extracts the public key from an EC_KEY structure
 * @key: Pointer to the EC_KEY structure to retrieve the public key from
 * @pub: Address at which to store the extracted public key (uncompressed)
 *
 * Return: Pointer to pub on success, NULL on failure
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
    const EC_POINT *pub_key_point;
    const EC_GROUP *group;
    size_t pub_key_len;

    /* Check if key is NULL */
    if (!key || !pub)
        return (NULL);

    /* Get the public key point from the EC_KEY */
    pub_key_point = EC_KEY_get0_public_key(key);
    if (!pub_key_point)
        return (NULL);

    /* Get the group from the EC_KEY */
    group = EC_KEY_get0_group(key);
    if (!group)
        return (NULL);

    /* Convert the public key point to uncompressed format */
    pub_key_len = EC_POINT_point2oct(group, pub_key_point,
                                     POINT_CONVERSION_UNCOMPRESSED,
                                     pub, EC_PUB_LEN, NULL);

    /* Check if conversion was successful */
    if (pub_key_len != EC_PUB_LEN)
        return (NULL);

    return (pub);
}
