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
/* Define the curve to use for EC operations */
#define EC_CURVE NID_secp256k1
/* Define the length of uncompressed public key (1 + 32 + 32 = 65 bytes) */
#define EC_PUB_LEN 65
#define EC_PRI_LEN 32
#define SIG_MAX_LEN 72


/**
 * struct sig_s - EC Signature structure
 * @sig: Signature buffer
 * @len: Signature size
 */
typedef struct sig_s
{
	uint8_t sig[SIG_MAX_LEN];
	uint8_t len;
} sig_t;

/* FUNCTIONS */
uint8_t *sha256(int8_t const *s,
		size_t len, uint8_t digest[SHA256_DIGEST_LENGTH]);
uint8_t *ec_sign(EC_KEY const *key,
	uint8_t const *msg, size_t msglen, sig_t *sig);
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN]);
EC_KEY *ec_load(char const *folder);
EC_KEY *ec_create(void);
int ec_save(EC_KEY *key, char const *folder);
int ec_verify(EC_KEY const *key, 
	uint8_t const *msg, size_t msglen, sig_t const *sig);

#endif /* HBLK_CRYPTO_H */
