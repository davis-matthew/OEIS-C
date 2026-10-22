/*
 * A348973  Numerator of ratio A129283(n) / A003959(n), where A003959 is
 *          multiplicative with a(p^e) = (p+1)^e and A129283(n) is sum of n
 *          and its arithmetic derivative.
 * https://oeis.org/A348973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A129283(n) / gcd(A003959(n), A129283(n))
 * Range:     u8 n <= 63, u16 n <= 9931, u32 n <= 9931, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A129283(n) = n + arithmetic_derivative(n)                     */
/*  A003959(n)  = ∏(p+1)^e  over n = ∏ p^e.                         */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* arithmetic derivative and A003959 on the fly */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t derivative = 0;          /* d(n) */
    seq_t a003959    = 1;          /* A003959(n) */

    idx_t temp = n;

    /* factor 2 separately for efficiency */
    if (temp % 2 == 0) {
        size_t e = 0;
        while ((temp % 2) == 0) {
            temp /= 2;
            ++e;
        }
        derivative += (seq_t)e * ((seq_t)n / (seq_t)2);
        seq_t fac = (seq_t)3;          /* 2 + 1 */
        for (size_t j = 0; j < e; ++j)
            a003959 *= fac;
    }

    /* trial division for odd primes up to sqrt(temp) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            derivative += (seq_t)e * ((seq_t)n / (seq_t)p);
            seq_t fac = (seq_t)(p + 1);
            for (size_t j = 0; j < e; ++j)
                a003959 *= fac;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        /* exponent is 1 */
        derivative += (seq_t)1 * ((seq_t)n / (seq_t)temp);
        seq_t fac = (seq_t)(temp + 1);
        a003959 *= fac;
    }

    seq_t a129283 = nn + derivative;   /* n + d(n) */
    seq_t g = gcd(a129283, a003959);
    return a129283 / g;
}
