#ifndef _ENDIANNESS_H_
#define _ENDIANNESS_H_

#include <stdint.h>
#include <stddef.h>

/**
 * union _endian_u - Union to check endianness
 * @i: Integer value
 * @bytes: Byte array
 */
typedef union _endian_u
{
    uint32_t i;
    uint8_t bytes[4];
} _endian_t;

/* Function prototypes */
uint8_t _get_endianness(void);
void _swap_endian(void *p, size_t size);

#endif /* _ENDIANNESS_H_ */

