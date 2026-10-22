/*
 * A327668  a(n) = n * Sum_{d|n} (-1)^(bigomega(d) - omega(d)) / d.
 * https://oeis.org/A327668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * Sum_{d|n} (-1)^(bigomega(d)-omega(d)) / d This
 *            multiplicative function satisfies a(p^e) = p^e +
 *            (p^e-(-1)^e)/(p+1) for a prime p and exponent e
 * Range:     u8 n <= 119, u16 n <= 9837, u32 n <= 9837, u64 n <= 9837,
 *            u128 n <= 9837, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* factorization of n */
    uint64_t primes[64];
    unsigned int exps[64];
    size_t cnt = 0;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt]   = p;
            exps[cnt]     = e;
            ++cnt;
        }
    }
    if (m > 1) {               /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* compute the product over prime powers */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned int e = exps[i];

        /* compute p^e */
        seq_t p_pow = (seq_t)1;
        for (unsigned int j = 0; j < e; ++j) {
            p_pow *= (seq_t)p;
        }

        /* term2 = (p_pow - (-1)^e) / (p+1) */
        seq_t denom = (seq_t)p + 1;
        seq_t term2;
        if ((e & 1) == 0) {           /* e even: (-1)^e = 1 */
            term2 = (p_pow - (seq_t)1) / denom;
        } else {                      /* e odd: (-1)^e = -1 */
            term2 = (p_pow + (seq_t)1) / denom;
        }

        seq_t f = p_pow + term2;      /* a(p^e) */
        result *= f;
    }

    return result;
}
