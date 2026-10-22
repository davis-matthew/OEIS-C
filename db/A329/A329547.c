/*
 * A329547  Number of natural numbers k <= n such that k^k is a square.
 * https://oeis.org/A329547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + ceil(floor(sqrt(n))/2)
 * Range:     u8 n <= 488, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of k ≤ n such that k^k is a perfect square. */
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, behaviour for n<1 irrelevant */

    seq_t half_n = (seq_t)n / (seq_t)2;    /* floor(n/2)          */

    idx_t sqrt_val = _floor_isqrt_u64(n); /* floor(sqrt(n))      */
    seq_t half_sqrt = ((seq_t)sqrt_val + (seq_t)1) / (seq_t)2; /* ceil(floor(sqrt(n))/2) */

    return half_n + half_sqrt;
}
