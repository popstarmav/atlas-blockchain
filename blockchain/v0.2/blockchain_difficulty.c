#include "blockchain.h"

/**
 * blockchain_difficulty - Computes the difficulty for the next block
 * @blockchain: Pointer to the Blockchain to analyze
 *
 * Return: The difficulty to be assigned to a potential next Block
 *
 * Description: Adjusts difficulty every DIFFICULTY_ADJUSTMENT_INTERVAL blocks
 * based on the actual time elapsed vs expected time between adjustment points.
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
    block_t *latest_block, *adjustment_block;
    uint32_t blockchain_size, latest_index;
    uint64_t expected_time, actual_time;
    uint32_t current_difficulty;

    if (!blockchain || !blockchain->chain)
        return (0);

    /* Get the latest block */
    latest_block = llist_get_tail(blockchain->chain);
    if (!latest_block)
        return (0);

    blockchain_size = llist_size(blockchain->chain);
    latest_index = latest_block->info.index;
    current_difficulty = latest_block->info.difficulty;

    /* Check if we need to adjust difficulty */
    /* Must be at adjustment interval AND not genesis block */
    if ((latest_index + 1) % DIFFICULTY_ADJUSTMENT_INTERVAL != 0 || 
        latest_index == 0)
    {
        return (current_difficulty);
    }

    /* Find the block from the last adjustment period */
    adjustment_block = llist_get_node_at(blockchain->chain, 
                                        blockchain_size - DIFFICULTY_ADJUSTMENT_INTERVAL);
    if (!adjustment_block)
        return (current_difficulty);

    /* Calculate expected vs actual time */
    expected_time = DIFFICULTY_ADJUSTMENT_INTERVAL * BLOCK_GENERATION_INTERVAL;
    actual_time = latest_block->info.timestamp - adjustment_block->info.timestamp;

    /* Adjust difficulty based on timing */
    if (actual_time < expected_time / 2)
    {
        /* Blocks are being mined too fast, increase difficulty */
        return (current_difficulty + 1);
    }
    else if (actual_time > expected_time * 2)
    {
        /* Blocks are being mined too slow, decrease difficulty */
        return (current_difficulty > 0 ? current_difficulty - 1 : 0);
    }

    /* Time is within acceptable range, keep same difficulty */
    return (current_difficulty);
}

