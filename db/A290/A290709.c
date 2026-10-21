/*
 * A290709  Number of irredundant sets in the complete tripartite graph
 *          K_{n,n,n}.
 * https://oeis.org/A290709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(2^n + n^2) - 2 for n > 1; a(1) = 4
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;

    /* 2^n */
    seq_t pow2n = pow2(n);

    /* n^2 */
    seq_t nn = (seq_t)n * (seq_t)n;

    /* (2^n + n^2) */
    seq_t sum = pow2n + nn;

    /* 3 * (2^n + n^2) */
    seq_t prod = sum * (seq_t)3;

    /* 3 * (2^n + n^2) - 2 */
    prod -= (seq_t)2;

    return prod;
}
