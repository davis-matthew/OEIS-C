/*
 * A385137  The sum of divisors d of n such that n/d is a 3-smooth number
 *          (A003586).
 * https://oeis.org/A385137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of divisors d of n such that n/d has no prime factor
 *            > 3 (i.e. n/d is a 3‑smooth number). The function factors n,
 *            separates the powers of 2 and 3, multiplies the remaining
 *            “large” prime powers into a base, and uses the multiplicative
 *            formula: a(n) = base × (2^(f2+1) – 1) × (3^(f3+1) –...
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not expected but safe */

    /* factor n by trial division */
    idx_t temp = n;
    idx_t f2 = 0, f3 = 0;          /* exponents of 2 and 3 */
    seq_t  base = 1;               /* product of primes > 3 */

    /* factor 2 */
    while ((temp & 1) == 0) {
        f2++;
        temp >>= 1;                /* temp /= 2; faster for powers of two */
    }

    /* factor 3 */
    while (temp % 3 == 0) {
        f3++;
        temp /= 3;
    }

    /* factor remaining primes > 3 */
    for (idx_t p = 5; p * p <= temp; p += 2) {
        idx_t e = 0;
        while (temp % p == 0) {
            e++;
            temp /= p;
        }
        if (e > 0) {
            /* compute p^e in seq_t */
            seq_t pe = 1;
            for (idx_t i = 0; i < e; ++i) {
                pe *= (seq_t)p;
            }
            base *= pe;
        }
    }

    /* if a prime > sqrt(n) remains */
    if (temp > 1) {
        base *= (seq_t)temp;
    }

    /* compute the multiplicative contribution of 2s and 3s */
    seq_t two_pow = 1;
    for (idx_t i = 0; i < f2 + 1; ++i) {
        two_pow *= (seq_t)2;          /* 2^(f2+1) */
    }

    seq_t three_pow = 1;
    for (idx_t i = 0; i < f3 + 1; ++i) {
        three_pow *= (seq_t)3;        /* 3^(f3+1) */
    }

    /* final result */
    seq_t result = base;
    result *= (two_pow - 1);
    seq_t three_half = (three_pow - 1) / 2;   /* integer division */
    result *= three_half;

    return result;
}
