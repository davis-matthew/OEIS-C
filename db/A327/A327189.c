/*
 * A327189  For any n >= 0: consider the different ways to split the binary
 *          representation of n into two (possibly empty) parts, say with
 *          value x and y; a(n) is the least possible value of x + y.
 * https://oeis.org/A327189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 1109, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = min over all binary splits of n of (x + y)
   where x is the value of the left part and y the value of the right part. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* compute the length L of the binary representation of n */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) { ++len; tmp >>= 1; }

    seq_t best = (seq_t)n;          /* split with empty left part */

    for (idx_t p = 1; p < len; ++p) /* p = number of bits in left part */
    {
        idx_t right_bits = len - p;
        seq_t left   = (seq_t)(n >> right_bits);
        uint64_t mask64 = ((uint64_t)1 << right_bits) - 1;
        seq_t right  = (seq_t)(n & mask64);
        seq_t sum = left + right;
        if (sum < best) best = sum;
    }

    return best;
}
