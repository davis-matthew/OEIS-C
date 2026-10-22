/*
 * A336776  a(n) is the least number of repetitions such that the result of
 *          the repeated execution of the multiplication f <- f*n started at
 *          f=1 produces an overflow, when the multiplication is performed
 *          using 32-bit single precision floats according to the IEEE 754
 *          standard.
 * https://oeis.org/A336776
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = the least k such that n^k > FLT_MAX, where FLT_MAX =
 *            (2^24−1)·2^104 = 2^128−2^104. Hence a(n) =
 *            ⌊log₂(FLT_MAX)/log₂(n)⌋ + 1
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2; for safety return 0 otherwise. */
    if (n < 2) return (seq_t)0;

    /* Constants used in the logarithmic formula.  They are computed only once. */
    static bool initialized = false;
    static long double log2FLTM;   /* log₂(FLT_MAX) */
    static long double log2_const; /* log₂(e), used to compute log₂(x) from log(x) */
    if (!initialized) {
        /* log₂(2) is the most convenient base conversion factor. */
        log2_const = logl(2.0L);
        /* log₂(FLT_MAX) = log₂(2^104·(2^24−1))
         *                = 104 + log₂(2^24−1) */
        log2FLTM = 104.0L + logl(16777215.0L) / log2_const;
        initialized = true;
    }

    /* Compute log₂(n) accurately in long double. */
    long double log2n = logl((long double)n) / log2_const;

    /* The exact threshold is log₂(FLT_MAX)/log₂(n), which is always < 128.
       Subtract a tiny epsilon to guard against round‑off that could move the
       value above an integer.  The sequence values are at most 128, so
       this subtraction cannot make the result negative. */
    const long double eps = 1e-12L;
    long double ratio = log2FLTM / log2n - eps;

    /* floor(ratio) gives the largest integer k with n^k ≤ FLT_MAX.
       The desired answer is one more than that. */
    idx_t k = (idx_t)floor(ratio);   /* use floor because ratio is positive */
    return (seq_t)(k + 1);
}
