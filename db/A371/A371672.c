/*
 * A371672  a(n) = floor(log_phi(n)) with phi = A001622.
 * https://oeis.org/A371672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_phi(n)) where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses long double arithmetic.
   It first estimates the exponent by a floating‑point
   division of logarithms and then corrects any rounding
   error by a couple of integer comparisons.  For n that
   fits into an idx_t the result always fits into a seq_t. */
#include <math.h>

seq_t a(idx_t n)
{
    /* golden ratio */
    static const long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;
    static const long double lnphi = logl(phi);

    /* n is always ≥ 1, but guard just in case */
    if (n < 1)
        return (seq_t)0;

    /* rough estimate using floating logarithms */
    long double est = logl((long double)n) / lnphi;
    idx_t k = (idx_t)floorl(est);

    /* correct a possible under‑estimation */
    while (1) {
        long double val = powl(phi, (long double)(k + 1));
        if (val <= (long double)n)
            ++k;
        else
            break;
    }

    /* correct a possible over‑estimation */
    while (1) {
        long double val = powl(phi, (long double)k);
        if (val > (long double)n)
            --k;
        else
            break;
    }

    return (seq_t)k;
}
