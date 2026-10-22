/*
 * A387359  For n >= 1, a(n) is the least k >= 0 such that n^2 - k^2 = 8*t
 *          where t >= 1 is a triangular number (A000217), or a(n) = -1 if no
 *          such k exists.
 * https://oeis.org/A387359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k ≥ 0 such that n² – k² = 8·t where t ≥ 1 is a
 *            triangular number, or –1 if no such k exists
 * Range:     i8 n <= 7, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation tests k from 0 up to n, checking whether the difference
   is a multiple of 8 and whether the quotient is a triangular number by
   verifying that 8·t + 1 is a perfect square. */

#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* cast index to seq_t for operations */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t sk = (seq_t)k;
        seq_t diff = sn * sn - sk * sk;   /* n² – k² */
        if (diff <= 0)                    /* diff must be positive */
            continue;
        if (diff % 8 != 0)                /* must be divisible by 8 */
            continue;
        seq_t t = diff / 8;
        if (t < 1)                        /* t must be at least 1 */
            continue;
        seq_t val = 8 * t + 1;            /* 8·t + 1 should be a square */
        seq_t sq  = floor_sqrt(val);
        if (sq * sq == val)               /* perfect square ⇒ t triangular */
            return sk;                    /* least k found */
    }
    return (seq_t)-1;                    /* no solution */
}
