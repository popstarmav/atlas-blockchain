#include "blockchain.h"

/**
 * block_hash - Computes the hash of a Block
 * @block: Pointer to the Block to be hashed
 * @digest: Buffer to store the computed hash
 *
 * Return: Pointer to @digest, or NULL on failure
 */
uint8_t *block_hash(block_t const *block, uint8_t digest[SHA256_DIGEST_LENGTH])
{
    size_t len;
    int8_t *buffer;

    if (!block || !digest)
        return (NULL);

    /* Calculate total length: info + data length */
    len = sizeof(block->info) + block->data.len;

    /* Allocate temporary buffer */
    buffer = malloc(len);
    if (!buffer)
        return (NULL);

    /* Copy block info */
    memcpy(buffer, &(block->info), sizeof(block->info));

    /* Copy block data if it exists */
    if (block->data.buffer && block->data.len > 0)
        memcpy(buffer + sizeof(block->info), block->data.buffer, block->data.len);

    /* Compute SHA256 hash */
    if (!sha256((int8_t const *)buffer, len, digest))
    {
        free(buffer);
        return (NULL);
    }

    free(buffer);
    return (digest);
}

