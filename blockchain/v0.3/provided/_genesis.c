#include "../blockchain.h"

/**
 * _genesis - Genesis block
 */
block_t const _genesis = {
    { /* info */
        0, /* index */
        0, /* difficulty */
        1537578000, /* timestamp */
        0, /* nonce */
        {0} /* prev_hash */
    },
    { /* data */
        "Holberton School", /* buffer - now char* instead of void* */
        16 /* len */
    },
    NULL, /* transactions */
    "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\x4d\x8d"
    "\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd4\xc2\x94\x24\x08\x5b\x24\xb6"
};

