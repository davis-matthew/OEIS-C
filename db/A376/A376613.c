/*
 * A376613  The binary expansion of a(n) tracks where the merge operations
 *          occur in a Tim sort algorithm applied to n blocks.
 * https://oeis.org/A376613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) is obtained by concatenating binary bits representing
 *            consecutive merge operations in a TimSort-like algorithm on n
 *            blocks. Each pair of blocks produces a '1' bit; if a block is
 *            left unchanged a '0' bit is appended. The bits are concatenated
 *            in the order the merges occur, with earlier-stage...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;          /* running binary value */
    idx_t  blocks = n;      /* current number of blocks */

    while (blocks > 1) {
        idx_t merges = blocks >> 1;     /* floor(blocks/2) merges */

        /* output one bit for each merge */
        for (idx_t i = 0; i < merges; ++i) {
            res <<= 1;
            res |= 1;
        }

        /* output a zero bit if a block is left unchanged */
        if (blocks & 1) {
            res <<= 1;           /* equivalent to appending a 0 bit */
        }

        /* prepare for next stage */
        blocks = merges + (blocks & 1);
    }
    return res;
}
