/*
 * A361810  a(n) is the sum of divisors of n that are both infinitary and
 *          exponential.
 * https://oeis.org/A361810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of divisors of n that are both infinitary and
 *            exponential (multiplicative: a(p^e) = Σ_{d|e, (d|e)==e} p^d )
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* raise a uint64_t base to an unsigned exponent, returning seq_t */
static inline seq_t pow_seq_uint64(uint64_t base, uint64_t exp)
{
    seq_t result = 1;
    for (uint64_t i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

/* compute a(p^e) */
static inline seq_t sum_prime_power(uint64_t p, uint64_t e)
{
    seq_t sum = 0;
    for (uint64_t d = 1; d <= e; ++d) {
        if ((e % d == 0) && (((d | e) == e)))
            sum += pow_seq_uint64(p, d);
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* multiplicative product over prime powers */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result *= sum_prime_power(primes[i], exps[i]);

    return result;
}
