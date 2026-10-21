/*
 * A287471  Number of connected dominating sets in the n-crown graph.
 * https://oeis.org/A287471
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - n - 1)^2 - n*(n-1)/2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined for indices < 3 */

    seq_t two_n = pow2(n);        /* 2^n  */
    seq_t sn = (seq_t)n;          /* cast n into seq_t */

    seq_t diff = two_n - sn - (seq_t)1;          /* (2^n - n - 1) */
    seq_t diff_sq = diff * diff;                /* (2^n - n - 1)^2 */
    seq_t half = sn * (sn - (seq_t)1) / (seq_t)2;/* n*(n-1)/2 */

    return diff_sq - half;
}
