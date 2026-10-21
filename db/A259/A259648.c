/*
 * A259648  a(n) = floor( prime(n)^3 / (n*log(n)) ).
 * https://oeis.org/A259648
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( prime(n)^3 / (n*log(n)) ) (OEIS A259648)
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For indices below the offset, behavior is irrelevant; return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Get the nth prime as a 64‑bit integer (sufficient for n ≤ 10 000). */
    uint64_t p = get_prime(n);

    /* Compute the quotient using long double for extra precision. */
    long double denom = (long double)n * log((long double)n);
    long double val   = (long double)p * (long double)p * (long double)p / denom;

    /* Floor to integer.  Add a tiny epsilon to guard against rounding
       errors that could drop the result by 1. */
    unsigned long long result = (unsigned long long)floor(val + 1e-12L);

    /* Convert to the generic integer type seq_t.  The conversion from
       unsigned long long is supported by the seq_t constructors. */
    return (seq_t)result;
}
