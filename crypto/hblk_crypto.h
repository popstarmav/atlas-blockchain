#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H

#include <stdint.h>
#include <stddef.h>
#include <openssl/sha.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/obj_mac.h>

/* Define the curve to use for EC operations */
#define EC_CURVE NID_secp256k1

/* FUNCTIONS */
uint8_t *sha256(int8_t const *s,
		size_t len, uint8_t digest[SHA256_DIGEST_LENGTH]);
EC_KEY *ec_create(void);

#endif /* HBLK_CRYPTO_H */
