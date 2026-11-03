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
 * Validated: standard, pass, 98 terms through n = 973 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, computes each a(p^e) using the
   closed form, and multiplies the results. */

#include <stdint.h>
#include <stddef.h>

/* Compute a(p^e) for a prime p and exponent e. */
static seq_t a_prime_pow(uint64_t p, uint64_t e)
{
    /* t = floor((e+3)/3) */
    idx_t t = (e + 3) / 3;

    /* Compute 1 + p^3 + p^6 + ... (t terms) */
    seq_t sum = (seq_t)1;
    seq_t term = (seq_t)1;
    seq_t p3 = (seq_t)p * (seq_t)p * (seq_t)p;
    for (idx_t i = 1; i < t; ++i) {   /* first term already 1 */
        term *= p3;
        sum += term;
    }

    /* Sign factor: -1 for p==2, +1 otherwise */
    seq_t part = sum * ((p == 2) ? (seq_t)-1 : (seq_t)1);

    /* Extra term for p==2 and e divisible by 3 */
    if ((p == 2) && ((e % 3) == 0)) {
        seq_t extra = (seq_t)1;
        for (uint64_t i = 0; i < e + 1; ++i) {
            extra *= (seq_t)2;
        }
        part += extra;
    }

    return part;
}

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
