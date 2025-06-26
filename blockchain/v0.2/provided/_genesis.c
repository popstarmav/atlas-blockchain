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
        {0}   /* prev_hash (all zeros) */
    },
    { /* data */
        (int8_t *)"Holberton School", /* buffer - static string */
        16  /* len */
    },
    /* hash */
    {
        0xc5, 0x2c, 0x26, 0xc8, 0xb5, 0x46, 0x16, 0x39,
        0x63, 0x5d, 0x8e, 0xdf, 0x2a, 0x97, 0xd4, 0x8d,
        0x0c, 0x8e, 0x00, 0x09, 0xc8, 0x17, 0xf2, 0xb1,
        0xd3, 0xd7, 0xff, 0x2f, 0x04, 0x51, 0x58, 0x03
    }
};

