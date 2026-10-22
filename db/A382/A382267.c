/*
 * A382267  Denominator of the harmonic mean of the exponents in the prime
 *          factorization of n.
 * https://oeis.org/A382267
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the harmonic mean of the exponents in the
 *            prime factorization of n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* For n < 2 the value is undefined in the OEIS definition;
       return 0 as a placeholder. */
    if (n < 2) return 0;

    /* Factor n into primes and exponents.  Allocate enough space
       for all possible prime factors (<64 for 64‑bit n). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    size_t k = cnt;
    if (k <= 1) return 1;   /* same exponent => harmonic mean = that exponent */

    /* Compute S = sum_{i} 1/e_i as a reduced fraction sum_num/sum_den */
    seq_t sum_num = 0;
    seq_t sum_den = 1;
    for (size_t i = 0; i < k; ++i) {
        seq_t e = (seq_t)exps[i];
        seq_t new_num = sum_num * e + sum_den;   /* a*e + b   */
        seq_t new_den = sum_den * e;             /* b*e       */
        seq_t g = gcd(new_num, new_den);         /* reduce */
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    /* Harmonic mean H = k / S = (k*sum_den) / sum_num. */
    seq_t H_num = (seq_t)k * sum_den;
    seq_t H_den = sum_num;
    seq_t g2 = gcd(H_num, H_den);
    seq_t denom = H_den / g2;   /* denominator of H in lowest terms */

    return denom;
}
