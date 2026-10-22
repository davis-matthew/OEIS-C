/*
 * A328373  Expansion of Sum_{k>=1} x^(k^2) * (1 + x^(2*k^2)) / (1 -
 *          x^(2*k^2))^2.
 * https://oeis.org/A328373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sum_{d|n, d odd, n/d square} d
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 6882, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A328373: sum of odd divisors d of n such that n/d is a perfect square. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence is defined for n>=1 */

    /* Factor out powers of 2.  The condition requires that
       the exponent of 2 in n be even, otherwise the result is 0.  */
    idx_t temp = n;
    idx_t two_exp = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        two_exp++;
    }
    if (two_exp & 1) return 0;

    /* Now deal with odd part of n. */
    idx_t remaining = temp;
    seq_t result = 1;

    idx_t p = 3;
    while (p * p <= remaining) {
        if (remaining % p == 0) {
            /* Count exponent of prime p in n. */
            idx_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                exp++;
            }

            /* Compute term for this prime factor. */
            seq_t pow_val = big_pow((seq_t)p, exp + 2);   /* p^(exp+2) */
            seq_t denom   = big_pow((seq_t)p, 2) - 1;     /* p^2 - 1 */
            seq_t term;
            if (exp & 1) {   /* odd exponent: term = (p^(exp+2) - p)/(p^2-1) */
                term = (pow_val - (seq_t)p) / denom;
            } else {        /* even exponent: term = (p^(exp+2) - 1)/(p^2-1) */
                term = (pow_val - 1) / denom;
            }
            result *= term;
        }
        p += 2;
    }

    /* If a prime factor remains > 1, it appears to the first power. */
    if (remaining > 1) {
        seq_t pow_val = big_pow((seq_t)remaining, 3);          /* p^3 */
        seq_t denom   = big_pow((seq_t)remaining, 2) - 1;      /* p^2 - 1 */
        seq_t term    = (pow_val - (seq_t)remaining) / denom;   /* odd exponent case */
        result *= term;
    }

    return result;
}
