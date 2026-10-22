/*
 * A373059  a(n) = Sum_{1 <= x_1, x_2 <= n} gcd(x_1, n)/gcd(x_1, x_2, n).
 * https://oeis.org/A373059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(p^e) = (p^(2*e)*((e+1)*p^2 + 2*p-e) + 1)/(p+1)^2,
 *            multiplicative over prime powers
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 268, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* small integer exponentiation in seq_t */
static seq_t pow_uint(seq_t base, uint64_t exp)
{
    seq_t res = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            res *= base;
        base *= base;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)      /* sequence undefined for n=0; return 0 safely */
        return (seq_t)0;

    /* factorise n into prime powers */
    static uint64_t primes_buf[64];
    static uint64_t exps_buf[64];
    size_t prime_count = 0;

    factor_u64(n, primes_buf, exps_buf, &prime_count);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_count; ++i)
    {
        /* prime p and exponent e of n */
        uint64_t p = primes_buf[i];
        uint64_t e = exps_buf[i];

        /* compute p^(2*e) */
        seq_t pow_p2e = pow_uint((seq_t)p, 2ULL * e);

        /* part = ((e+1)*p^2 + 2*p - e) */
        seq_t pp   = (seq_t)p * (seq_t)p;
        seq_t part = ((seq_t)(e + 1) * pp) + (seq_t)(2U * p) - (seq_t)e;

        /* numerator = pow_p2e * part + 1 */
        seq_t numerator = pow_p2e * part + (seq_t)1;

        /* denominator = (p+1)^2 */
        seq_t denom = (seq_t)(p + 1U) * (seq_t)(p + 1U);

        /* a(p^e) = numerator / denom (exact division) */
        seq_t ap = numerator / denom;

        result *= ap;
    }

    return result;
}
