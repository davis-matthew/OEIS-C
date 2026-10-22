/*
 * A309736  a(1) = 1, and for any n > 1, a(n) is the least k > 0 such that the
 *          binary representation of n^k starts with "10".
 * https://oeis.org/A309736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k>0 such that the binary representation of n^k
 *            starts with "10". The test is performed by checking the
 *            fractional part of k·log₂(n): frac(k·log₂(n)) < log₂(1.5) ≈
 *            0.5849625007 … The implementation uses `long double` arithmetic
 *            to avoid the equality problem that occurs with plain doub...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0 or 1 are trivially 1. */
    if (n <= 1) return (seq_t)1;

    /* Powers of 2 always satisfy the condition immediately. */
    if ((n & (n - 1)) == 0) return (seq_t)1;

    /* High‑precision constants. */
    const long double LOG2_1_5 = log2l(1.5L);          /* ≈0.5849625007… */
    const long double LOG2_N   = log2l((long double)n);/* ≈log₂(n) */

    long double frac = 0.0L;                          /* fractional part of k·log₂(n) */

    /* Search for the minimal k.  The expected k is small (≤10 for n≤10000). */
    for (idx_t k = 1; ; ++k) {
        frac += LOG2_N;                               /* add one more multiple */
        if (frac >= 1.0L)
            frac -= floorl(frac);                     /* keep only fractional part */

        /*
         * The leading bits are "10" iff the fractional part is strictly
         * less than log₂(1.5).  A tiny epsilon is subtracted to guard
         * against rounding errors that could make a value exactly
         * equal to the boundary appear smaller.
         */
        if (frac + 1e-18L < LOG2_1_5)
            return (seq_t)k;
    }
}
