/*
 * A309707  a(n) is the least integer k > 1 such that k^n starts with 1.
 * https://oeis.org/A309707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the least integer k>1 such that k^n starts with digit 1. */

#include <math.h>

/* Computes A309707:  a(n) = min{k>1 | the leading digit of k^n is 1}. */
seq_t a(idx_t n)
{
    /* For n=0 the sequence is undefined, but we simply return 0. */
    if (n == 0)
        return (seq_t)0;

    const long double thr = log10l(2.0L);   /* log10(2)  ≈ 0.301029995664 */
    const long double eps = 1e-15L;         /* safety margin for rounding */

    for (int k = 2; k <= 10; ++k) {
        /* Compute fractional part of n*log10(k) */
        long double val = (long double)n * log10l((long double)k);
        long double frac = val - floorl(val);
        /* Check if the leading digit is 1: 10^frac ∈ [1,2) */
        if (frac + eps < thr)
            return (seq_t)k;
    }
    /* In all cases a(n)≤10, so 10 is reached only for n=1. */
    return (seq_t)10;
}
