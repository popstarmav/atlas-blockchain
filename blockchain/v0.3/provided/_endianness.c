#include "endianness.h"

/**
 * _get_endianness - Get system endianness
 *
 * Return: 1 for little endian, 2 for big endian, 0 for unknown
 */
uint8_t _get_endianness(void)
{
    _endian_t order = { 1 };

    if (order.bytes[0] == 1)
        return (1); /* Little endian */
    if (order.bytes[0] == 0)
        return (2); /* Big endian */

    return (0); /* Should not happen */
}

/**
 * _swap_endian - Swap endianness of a buffer
 * @p: Pointer to buffer
 * @size: Size of buffer
 */
void _swap_endian(void *p, size_t size)
{
    size_t i;
    uint8_t *n = (uint8_t *)p;

    for (i = 0; i < size / 2; i++)
    {
        n[i] ^= n[size - (i + 1)];
        n[size - (i + 1)] ^= n[i];
        n[i] ^= n[size - (i + 1)];
    }
}

