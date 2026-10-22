/*
 * A368929  Dirichlet g.f.: zeta(s-2)^2 * (1 - 2^(3-s)) / zeta(s).
 * https://oeis.org/A368929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 94, i32 n <= 9239, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A368929 – Dirichlet multiplicative function.
 *  Formula: a(1) = 1;
 *           multiplicatively:
 *           a(2^e) = -(3e-2) * 2^(2e-2);
 *           a(p^e) = p^(2e-2) * (p^2 + e*(p^2-1))
 *           for every odd prime p and exponent e ≥ 1. */

#include <stdint.h>

static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

static seq_t prime_power_contribution(idx_t p, idx_t e)
{
    if (p == 2) {
        /* a(2^e) = -(3e-2) * 2^(2e-2) */
        seq_t pow2 = seq_pow((seq_t)2, 2 * e - 2);
        seq_t mult = (seq_t)(3 * e - 2);
        return -mult * pow2;
    } else {
        seq_t p_sq = (seq_t)p * (seq_t)p;
        seq_t pow_part = seq_pow((seq_t)p, 2 * e - 2);
        seq_t inner = p_sq + (seq_t)e * (p_sq - 1);
        return pow_part * inner;
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t result = 1;
    idx_t temp = n;

    /* factor out powers of 2 */
    idx_t e = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0)
        result *= prime_power_contribution(2, e);

    /* factor odd primes by trial division */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= prime_power_contribution(p, e);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
        result *= prime_power_contribution(temp, 1);

    return result;
}
