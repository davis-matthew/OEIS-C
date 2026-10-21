/*
 * A245776  Numerator of (n/tau(n) - sigma(n)/n).
 * https://oeis.org/A245776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = reduced numerator of (n/tau(n) - sigma(n)/n)
 * Range:     i8 n <= 12, i16 n <= 184, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* tau(n) = number of divisors */
    uint64_t tau = divisor_count_u64(n);
    /* sigma(n) = sum of divisors */
    seq_t sigma = sigma_of(n);

    /* Compute numerator: n^2 - tau * sigma */
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t denom = (seq_t)n * (seq_t)tau;
    seq_t numerator = nn - (seq_t)tau * sigma;

    /* Reduce the fraction numerator/denom to lowest terms */
    seq_t g;
    if (numerator < 0)
        g = gcd(-numerator, denom);
    else
        g = gcd(numerator, denom);

    return numerator / g;
}
