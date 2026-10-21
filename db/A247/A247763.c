/*
 * A247763  The number of representations of n as n=x^2-2^y, x>0, y>=0.
 * https://oeis.org/A247763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 136, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   bitlen_seq_t, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of representations of n as x^2 - 2^y
 * with x > 0 and y >= 0.  The function counts all integer pairs
 * (x, y) satisfying this equation.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence's integer type */
    seq_t n0 = (seq_t)n;
    /* Count of representations */
    seq_t count = 0;

    /* Upper bound on y: 2 * floor(log2(n)) + 5.
     * bitlen_seq_t gives ⌊log2(x)⌋+1 for x>0, so subtract 1. */
    unsigned bl = bitlen_seq_t(n0);          /* bl ≥ 1 for n ≥ 1 */
    unsigned max_y = 2 * (bl - 1) + 5;        /* safe upper bound */

    seq_t pow2 = 1;  /* 2^y */
    for (idx_t y = 0; y <= max_y; ++y) {
        seq_t sum = n0 + pow2;                /* s^2 = n + 2^y */
        seq_t s = floor_sqrt(sum);
        if (s * s == sum) {
            /* exact square found → a solution (x = s, y = current). */
            count++;
        }
        /* Prepare next power of two: 2^{y+1} */
        pow2 <<= 1;
    }
    return count;
}
