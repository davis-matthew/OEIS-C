/*
 * A259913  Discriminant of the number field containing the number with
 *          periodic continued fraction [1,n,1,n,1,n,...].
 * https://oeis.org/A259913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fundamental discriminant of the quadratic field *
 *            containing the quadratic irrational whose continued fraction is
 *            * [1,n,1,n,1,n,...]. The discriminant of the order defined by *
 *            the minimal polynomial n·x² – n·x – 1 is Δ = n·(n+4). Removing
 *            all * square factors from Δ yields a square‑free...
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for n==0 is irrelevant */

    seq_t ns = (seq_t)n;
    seq_t d = ns * (ns + (seq_t)4);   /* Δ = n·(n+4) */

    /* Remove all square factors from d */
    for (seq_t p = (seq_t)2; p * p <= d; ++p) {
        /* Divide out any factor p² while possible */
        seq_t p2 = p * p;
        while (d % p2 == 0) {
            d /= p2;
        }
    }

    /* Compute the fundamental discriminant */
    if (d % (seq_t)4 == (seq_t)1) {
        return d;
    } else {
        return d * (seq_t)4;
    }
}
