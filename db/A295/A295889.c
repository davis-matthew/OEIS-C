/*
 * A295889  a(n) = 1 if binary weights of n and 3n have the same parity, 0
 *          otherwise.
 * https://oeis.org/A295889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if binary weights of n and 3*n have the same parity, 0
 *            otherwise
 * Range:     u8 n <= 85, u16 n <= 8797, u32 n <= 8797, u64 n <= 8797,
 *            u128 n <= 8797, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Parity of the binary weight (popcount) of a seq_t value.
   Returns 0 for even popcount, 1 for odd popcount. */
static int popcount_parity(seq_t x)
{
    int p = 0;
    while (x) {
        p ^= (int)(x & 1);
        x >>= 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    int par_n   = popcount_parity(n_seq);
    seq_t three_n = n_seq * 3;
    int par_3n  = popcount_parity(three_n);
    return (par_n == par_3n) ? 1 : 0;
}
