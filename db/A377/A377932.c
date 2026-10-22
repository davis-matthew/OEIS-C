/*
 * A377932  a(n) is the denominator of the third convergent of the continued
 *          fraction of n^(1/n).
 * https://oeis.org/A377932
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the third convergent of the continued
 *            fraction of n^(1/n)
 * Range:     u8 n <= 53, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The function assumes n >= 2. For n < 2 it returns 0.  */
#include <math.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Compute x = n^(1/n) using double precision. */
    double dn = (double)n;
    double x = pow(dn, 1.0 / dn);

    /* First remainder = x - 1. */
    double r0 = x - 1.0;
    if (r0 <= 0.0) return 0;

    /* a1 = floor(1 / r0).  The small epsilon protects against
       rounding just below an integer. */
    unsigned long long a1 = (unsigned long long)
        floor(1.0 / r0 + 1e-12);

    /* Second remainder. */
    double r1 = 1.0 / r0 - (double)a1;
    if (r1 <= 0.0) return 0;

    unsigned long long a2 = (unsigned long long)
        floor(1.0 / r1 + 1e-12);

    /* Denominator of the third convergent: q2 = a2 * a1 + 1. */
    unsigned __int128 prod = (unsigned __int128)a2 * a1 + 1;
    return (seq_t)prod;
}
