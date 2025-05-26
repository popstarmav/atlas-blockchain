#include "hblk_crypto.h"

/**
 * ec_create - Creates a new EC key pair
 *
 * Return: Pointer to an EC_KEY structure containing both public 
 * and privatev keys, or NULL upon failure
 */
EC_KEY *ec_create(void)
{
    EC_KEY *key = NULL;
    
    /* Create a new EC_KEY structure */
    key = EC_KEY_new_by_curve_name(EC_CURVE);
    if (!key)
        return (NULL);
    
    /* Generate the key pair (both private and public keys) */
    if (EC_KEY_generate_key(key) != 1)
    {
        EC_KEY_free(key);
        return (NULL);
    }
    
    return (key);
}

