/*
 * A360111  a(n) = 1 if there is no prime p such that p^p divides n, but for
 *          the arithmetic derivative of n such a prime exists, otherwise a(n)
 *          = 0; a(1) = 0 by convention.
 * https://oeis.org/A360111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has no prime p with p^p | n but its arithmetic
 *            derivative n' has some prime q with q^q | n', else 0; a(1) = 0
 *            by convention
 * Range:     u8 n <= 2184, u16 n <= 9929, u32 n <= 9929, u64 n <= 9929,
 *            u128 n <= 9929, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Provided by the build environment */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/pow_uint.h"     /* pow_uint(uint64_t base, idx_t exp) returns seq_t */

/* Small primes needed for checking q^q | derivative (q^q grows very fast) */
static const uint64_t small_primes[] = {2, 3, 5, 7, 11, 13};
static const size_t small_primes_cnt = sizeof(small_primes) / sizeof(small_primes[0]);

/* Arithmetic derivative: D(n) = Σ e_i * n / p_i */
static seq_t arithmetic_derivative(idx_t n)
{
    uint64_t pr[32], ex[32];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t deriv = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)ex[i] * (seq_t)(n / pr[i]); /* n divisible by pr[i] */
        deriv += term;
    }
    return deriv;
}

/* Returns 1 if an integer x has a prime q with q^q | x, otherwise 0. */
/* Only small primes are necessary for n up to ~10^4 (see analysis). */
static int has_prime_power_divisor(seq_t x)
{
    for (size_t i = 0; i < small_primes_cnt; ++i) {
        uint64_t q = small_primes[i];
        seq_t qpow = pow_uint(q, (idx_t)q);          /* q^q */
        if (qpow > x)
            break;                                 /* no larger q can divide x */
        if (x % qpow == 0)
            return 1;
    }
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)          /* a(1) = 0 by convention */
        return (seq_t)0;

    uint64_t pr[32], ex[32];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Check for any p with exponent >= p (i.e., p^p | n) */
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] >= pr[i])
            return (seq_t)0;
    }

    /* Compute arithmetic derivative n' */
    seq_t deriv = arithmetic_derivative(n);

    /* Check if derivative has a prime power divisor q^q */
    if (has_prime_power_divisor(deriv))
        return (seq_t)1;
    else
        return (seq_t)0;
}
