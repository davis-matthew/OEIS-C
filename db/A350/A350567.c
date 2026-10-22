/*
 * A350567  a(n) is the maximum number of key comparisons required to perform
 *          an indirect sort of n records with distinct keys using a two-way
 *          merge (A. D. Woodall's mergesort).
 * https://oeis.org/A350567
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   Let m = ceil(log2(n)). base = (n+1)*m - 2^m + 1. Then: if n <=
 *            6: a(n) = base - 1; else if 7 <= n <= 8: a(n) = base; else: a(n)
 *            = base + floor((n-9)/2)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)        /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* compute ceil(log2(n)) */
    idx_t m = 0;
    idx_t tmp = n - 1;
    while (tmp > 0) {
        m++;
        tmp >>= 1;
    }

    /* base = (n+1)*m - 2^m + 1, all computations in seq_t */
    seq_t base = (seq_t)(n + 1) * (seq_t)m
                 - (((seq_t)1) << m) + (seq_t)1;

    if (n <= 6)
        return base - (seq_t)1;
    if (n >= 9) {
        idx_t add = (n - 9) >> 1;          /* floor((n-9)/2) */
        return base + (seq_t)add;
    }
    return base;                           /* n == 7 or 8 */
}
