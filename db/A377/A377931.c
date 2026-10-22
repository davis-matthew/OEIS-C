/*
 * A377931  a(n) is the numerator of the third convergent of the continued
 *          fraction of n^(1/n).
 * https://oeis.org/A377931
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of third convergent of the simple continued *
 *            fraction of n^(1/n). a(2) = 7, a(3) = 10, a(4) = 7, …
 * Range:     u8 n <= 53, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* This implementation is valid for all n ≥ 2.  For n < 2
     * the OEIS offset is not defined; we simply return 0. */
    if (n < 2)
        return 0;

    /* Compute x = n^(1/n) using long double for extra precision. */
    long double ln_n = logl((long double)n);
    long double x = expl(ln_n / (long double)n);

    /* a0 = 1 because 1 < x < 2 for all integer n ≥ 2. */
    (void)0; /* placeholder to keep a0 implicit */

    /* First partial quotient: a1 = floor(1/(x-1)). */
    long double y = 1.0L / (x - 1.0L);
    const long double EPS = 1e-15L; /* safety margin for rounding */
    unsigned long long a1 = (unsigned long long)floorl(y + EPS);

    /* Second partial quotient: a2 = floor(1/(y - a1)). */
    long double frac = y - (long double)a1;
    unsigned long long a2 = (unsigned long long)floorl(1.0L / (frac + EPS));

    /* The numerator of the third convergent is
     *   p2 = a2 * (a1 + 1) + 1. */
    seq_t numerator = (seq_t)a2 * ((seq_t)a1 + 1) + 1;

    return numerator;
}
