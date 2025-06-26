#include "blockchain.h"

/**
 * hash_matches_difficulty - checks if a hash matches a given difficulty
 * @hash: the hash to check
 * @difficulty: the minimum difficulty the hash should match
 *
 * Return: 1 if the difficulty is respected, 0 otherwise
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
		uint32_t difficulty)
{
	uint32_t leading_zeros = 0;
	uint32_t byte_index = 0;
	uint8_t current_byte;
	int bit_index;

	if (!hash)
		return (0);

	/* Count leading zero bits */
	while (byte_index < SHA256_DIGEST_LENGTH && leading_zeros < difficulty)
	{
		current_byte = hash[byte_index];

		/* If entire byte is zero, add 8 to count */
		if (current_byte == 0)
		{
			leading_zeros += 8;
			byte_index++;
			continue;
		}
		
		/* Count leading zero bits in current byte */
		for (bit_index = 7; bit_index >= 0; bit_index--)
		{
			if ((current_byte >> bit_index) & 1)
				break;
			leading_zeros++;
		}
		break;
	}

	return (leading_zeros >= difficulty ? 1 : 0);
}

