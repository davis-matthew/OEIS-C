/*
 * A267310  a(n) is the numerator of Sum_{d|n} sigma(n/d)^d/d, where sigma is
 *          A000203.
 * https://oeis.org/A267310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of \sum_{d|n} sigma(n/d)^d / d
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 161,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _SEQ_A267310_H_
#define _SEQ_A267310_H_

/* seq_t and idx_t are defined by the build system.  */
#include <stdint.h>   /* for uint64_t etc.  */
#include "../utils/gcd_seq.h"

/* Local helper: compute sigma(n) = sum of divisors of n. */
static seq_t sigma_of(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 1;
    idx_t tmp = n;

    for (idx_t p = 2; (idx_t)p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            seq_t term = 1;          /* sum 1 + p + p^2 + ...  */
            seq_t p_pow = 1;         /* p^k   */
            while (tmp % p == 0) {
                tmp /= p;
                p_pow *= (seq_t)p;    /* p^k */
                term += p_pow;        /* add p^k to term */
            }
            result *= term;
        }
    }
    if (tmp > 1) {                   /* remaining prime factor */
        result *= (seq_t)1 + (seq_t)tmp;
    }
    return result;
}

/* Local helper: exponentiation by squaring on seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Local helper: Euclidean gcd on seq_t. */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t N = 0;                     /* N = sum_{d|n} sigma(n/d)^d * (n/d)  */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* Process divisor d */
            idx_t m1 = n / d;
            seq_t sigma1 = sigma_of(m1);
            seq_t term1 = pow_seq(sigma1, d) * (seq_t)m1;
            N += term1;

            /* Process the complementary divisor if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t m2 = n / d2;   /* m2 = d */
                seq_t sigma2 = sigma_of(m2);
                seq_t term2 = pow_seq(sigma2, d2) * (seq_t)m2;
                N += term2;
            }
        }
    }

    seq_t g = gcd_seq(N, (seq_t)n);
    return N / g;                    /* numerator of the reduced fraction */
}

#endif /* _SEQ_A267310_H_ */
