/*
 * A270861  Irregular triangle read by rows: numerators of the coefficients of
 *          polynomials J(2n-1,z) = Sum_(k=1,2, .. n) ((n+1)^2 - k +
 *          (n+1-k)*z^n)*z^(k-1)/k.
 * https://oeis.org/A270861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, frac
 * Formula:   a(n) = Numerators of the coefficients of J(2n-1,z) =
 *            Sum_{k=1..n} ((n+1)^2-k+(n+1-k)*z^n)*z^{k-1}/k, read by rows
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/gcd.h"

static inline idx_t find_row(idx_t n)
{
    /* Find minimal r such that r*(r+1) >= n */
    if (n == 0) return 0;   /* should not be used, but keep safe */
    long double root = sqrtl((long double)n * 4.0L + 1.0L);
    idx_t r = (idx_t)((root - 1.0L) / 2.0L);   /* floor */
    /* Adjust if necessary (possible due to rounding) */
    while ( ( (__int128)r * (r + 1) ) < n ) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t r = find_row(n);                /* row number (>=1) */
    idx_t prev_terms = r * (r - 1);       /* terms before this row */
    idx_t j = n - 1 - prev_terms;         /* zero‑based offset in row */
    idx_t k;
    seq_t numer;

    if (j < r) {                          /* first half of the row */
        k = j + 1;
        seq_t top = ((seq_t)(r + 1) * (r + 1)) - k;
        seq_t d = gcd(top, k);
        numer = top / d;
    } else {                             /* second half of the row */
        k = (j - r) + 1;
        seq_t top = (seq_t)(r + 1) - k;
        seq_t d = gcd(top, k);
        numer = top / d;
    }
    return numer;
}
