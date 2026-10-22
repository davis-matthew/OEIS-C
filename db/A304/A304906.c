/*
 * A304906  L.g.f.: log(Product_{k>=1} (1 + x^(k^3))) = Sum_{n>=1} a(n)*x^n/n.
 * https://oeis.org/A304906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = multiplicative; for prime power p^e, a(p^e) = (-1)^(p+1)
 *            * (p^(3*floor((e+3)/3)) - 1)/(p^3 - 1) + [p==2 && e%3==0 ?
 *            2^(e+1) : 0]
 * Range:     i8 n <= 215, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   a_prime_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, computes each a(p^e) using the
   closed form, and multiplies the results. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a_prime_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n≥1 in practice, but guard */

    uint64_t m = (uint64_t)n;
    /* arrays for prime factors and exponents; max 64 factors enough for n≤10000 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;

    /* Trial division factorization */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[count] = p;
            exps[count]   = e;
            ++count;
        }
    }
    if (m > 1) {                    /* remaining prime factor */
        primes[count] = m;
        exps[count]   = (uint64_t)1;
        ++count;
    }

    /* Multiplicative product of a(p^e) values */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        result *= a_prime_pow(primes[i], exps[i]);
    }

    return result;
}
