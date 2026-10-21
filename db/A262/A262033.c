/*
 * A262033  Number of permutations of [n] beginning with at least floor(n/2)
 *          ascents.
 * https://oeis.org/A262033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k = ceil((n+1)/2)+1}^{n} k (equivalently a(n) = n! /
 *            ceil((n+1)/2)!) Base cases: a(0) = a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 30, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n == 0 or 1, the sequence is defined as 1 */
    if (n <= 1)
        return (seq_t)1;

    /* Compute start = ceil((n+1)/2) + 1
       ceil((n+1)/2) = (n+2)/2 (integer division) */
    idx_t start = (n + 2) / 2 + 1;

    seq_t prod = (seq_t)1;
    for (idx_t k = start; k <= n; ++k)
        prod *= (seq_t)k;

    return prod;
}
