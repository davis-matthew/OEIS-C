/*
 * A267323  The number of permutations in S_n with strategic pile of size 3.
 * https://oeis.org/A267323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ((n-3)² + 2) · (n-3)! for n ≥ 4, a(n) = 0 for n < 4
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    idx_t k = n - 3;                 /* k = n-3 ≥ 1 */
    seq_t fac = factorial(k);        /* (n-3)! as seq_t */

    seq_t kk = (seq_t)k;
    seq_t mult = kk * kk + (seq_t)2; /* (n-3)² + 2 as seq_t */

    return mult * fac;
}
