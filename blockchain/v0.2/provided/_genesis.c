#include "../blockchain.h"

/**
 * _genesis - Genesis block
 */
const block_t _genesis = {
    { /* info */
        0,    /* index */
        0,    /* difficulty */
        1537578000, /* timestamp */
        0,    /* nonce */
        {0}   /* prev_hash */
    },
    { /* data */
        "Holberton School", /* buffer */
        16  /* len */
    },
    /* hash */
    "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
    "\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"
};

