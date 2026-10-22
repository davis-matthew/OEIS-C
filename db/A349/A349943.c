/*
 * A349943  Number of ways to write n as a^4 + (b^4 + c^2 + d^2)/9, where
 *          a,b,c,d are nonnegative integers with c <= d.
 * https://oeis.org/A349943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a^4 + (b^4 + c^2 + d^2)/9 *
 *            where a,b,c,d are nonnegative integers with c <= d
 * Range:     u8 n <= 7, u16 n <= 1723, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required utilities for square root and integer types. */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* convert index to seq_t once */
    seq_t count = 0;                /* result accumulator */

    /* Loop over all possible a such that a^4 <= n. */
    for (idx_t a = 0; ; ++a) {
        seq_t a4 = (seq_t)a * a * a * a;   /* a^4 */
        if (a4 > nn) break;               /* a^4 has exceeded n */

        /* For a fixed a, we need 9*n = 9*a^4 + b^4 + c^2 + d^2 */
        /* Hence the remaining amount to be distributed among b,c,d is */
        seq_t target = 9 * nn - 9 * a4;     /* 9*(n - a^4) */

        /* Loop over all possible b such that b^4 <= target. */
        for (idx_t b = 0; ; ++b) {
            seq_t b4 = (seq_t)b * b * b * b;   /* b^4 */
            if (b4 > target) break;            /* b^4 too large */

            /* The rest must be representable as c^2 + d^2. */
            seq_t rest = target - b4;          /* rest >= 0 */

            /* Iterate over non‑negative c.  We only need c <= d, so */
            /* c^2 must not exceed rest.  c is bounded by sqrt(rest). */
            idx_t max_c = (idx_t)floor_sqrt(rest);
            for (idx_t c = 0; c <= max_c; ++c) {
                seq_t c2 = (seq_t)c * c;
                seq_t d2 = rest - c2;        /* d^2 = rest - c^2 */

                seq_t d = floor_sqrt(d2);    /* integer square root of d² */

                /* Check that d^2 equals d2 and enforce c <= d. */
                if (d * d == d2 && d >= (seq_t)c) {
                    ++count;
                }
            }
        }
    }
    return count;                     /* number of representations */
}
