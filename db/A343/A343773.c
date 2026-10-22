/*
 * A343773  Excess of the number of even Motzkin n-paths (A107587) over the
 *          odd ones (A343386).
 * https://oeis.org/A343773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((2*n+1)*a(n-1) - 5*(n-1)*a(n-2))/(n+2), with a(0) = 1,
 *            a(1) = 1. Recurrence is integer‐valued for all n
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 33, i64 n <= 62, i128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial two terms. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
        /* In C, use 128‑bit intermediates to avoid overflow of fixed basis
           types (int8_t … int64_t).  The large intermediary still
           results in a seq_t value after the integer division. */
        __int128 ti   = (__int128)i;
        __int128 num  = (ti * 2 + 1) * (__int128)prev1
                       - (__int128)5 * (ti - 1) * (__int128)prev2;
        cur = (seq_t)(num / (ti + 2));
#else
        /* For C++ builds (including builds with an arbitrary‑precision
           seq_t) fall back to pure seq_t arithmetic.  The implementation
           still yields the correct integer division result. */
        seq_t num = ((seq_t)(2 * i + 1) * prev1)
                 - ((seq_t)5 * (seq_t)(i - 1) * prev2);
        cur = num / (seq_t)(i + 2);
#endif
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
