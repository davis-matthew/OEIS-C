/*
 * A275119  The even integers in the sequence appear in blocks and the
 *          successive sizes of these blocks are given by the sequence itself.
 * https://oeis.org/A275119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This implementation builds the sequence A275119 by generating
 *            blocks of consecutive even numbers followed by equal‑sized
 *            blocks of consecutive odd numbers. The block sizes are taken
 *            from the sequence itself: block i has size a(i). The sequence
 *            begins with 1 and follows the greedy construction des...
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for invalid offset */
    seq_t *seq = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!seq) return (seq_t)0;            /* allocation failed */
    seq[1] = (seq_t)1;
    idx_t pos = 2;                        /* next free index in seq */
    idx_t block = 1;                      /* current block number */
    seq_t next_even = (seq_t)2;
    seq_t next_odd  = (seq_t)3;

    while (pos <= n)
    {
        seq_t blk_size = seq[block];           /* size of this block */
        /* place evens of the block */
        for (idx_t k = 0; k < (idx_t)blk_size && pos <= n; ++k)
        {
            seq[pos++] = next_even;
            next_even += (seq_t)2;
        }
        /* place odds of the block */
        for (idx_t k = 0; k < (idx_t)blk_size && pos <= n; ++k)
        {
            seq[pos++] = next_odd;
            next_odd += (seq_t)2;
        }
        ++block;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
