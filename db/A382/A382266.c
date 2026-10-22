/*
 * A382266  Numerator of the harmonic mean of the exponents in the prime
 *          factorization of n.
 * https://oeis.org/A382266
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the harmonic mean of the exponents in the
 *            prime factorization of n (OEIS A382266)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* Undefined for n<2; offset starts at 2 */

    /* Factor n into primes and exponent counts.  Allocate a sufficient
       small buffer for the typical number of distinct prime factors of
       a 64‑bit integer. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute D = product of all exponents. */
    seq_t D = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        D *= (seq_t)ex[i];
    }

    /* Sum of reciprocals expressed as sum = Σ D / e_i. */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += D / (seq_t)ex[i];
    }

    /* Harmonic mean = k / (Σ 1/e_i) = k*D / sum.  Reduce the fraction
       to obtain the numerator. */
    seq_t numerator = (seq_t)cnt * D;
    seq_t g = gcd(numerator, sum);
    return numerator / g;
}
