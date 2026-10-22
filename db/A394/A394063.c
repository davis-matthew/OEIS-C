/*
 * A394063  a(n) = n^n * numerator(hypergeom([1, -n], [], -1/(n + 1))).
 * https://oeis.org/A394063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^n * numerator hypergeom([1, -n], [], -1/(n + 1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)             /* 0^0 is treated as 1; hypergeometric term = 1 */
        return (seq_t)1;

    seq_t sn   = (seq_t)n;           /* n in seq_t */
    seq_t np1  = sn + (seq_t)1;      /* n + 1 */

    /* n^n */
    seq_t n_to_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        n_to_n *= sn;

    /* (n+1)^n  (common denominator) */
    seq_t denom = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        denom *= np1;

    /* Compute the numerator of the hypergeometric series:
       sum_{k=0}^{n} n!/(n-k)! * (n+1)^{n-k} = sum_{k=0}^{n} falling * pow_term
    */
    seq_t falling  = (seq_t)1;  /* n!/(n-k)! for current k */
    seq_t pow_term = denom;     /* (n+1)^{n-k} starting with k=0 */
    seq_t sum_N    = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow_term * falling;
        sum_N += term;

        if (k == n) break;          /* avoid overflow after last step */

        falling *= (sn - (seq_t)k); /* multiply by (n - k) for next k */
        pow_term /= np1;            /* exact division by (n + 1) */
    }

    /* Reduce the fraction: numerator = sum_N / gcd(sum_N, denom) */
    seq_t g          = gcd(sum_N, denom);
    seq_t numerator  = sum_N / g;

    /* Final result: n^n * numerator */
    seq_t result = n_to_n * numerator;
    return result;
}
