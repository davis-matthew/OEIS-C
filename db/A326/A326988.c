/*
 * A326988  Sum of nonpowers of 2 dividing n.
 * https://oeis.org/A326988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) - Sum_{k=0}^{v2(n)} 2^k where sigma(n) is the
 *            sum of all divisors of n and v2(n) is the exponent of 2 in n's
 *            prime factorisation
 * Range:     u8 n <= 16, u16 n <= 4908, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* local helper: compute base^exp for seq_t */
static inline seq_t seq_pow_seq(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    /* handle n == 0 (undefined in OEIS) safely */
    if (n == 0)
        return 0;

    /* compute v2(n) (exponent of 2) */
    idx_t v = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        ++v;
        temp >>= 1;
    }

    /* sum of powers of 2 dividing n: 2^{v+1} - 1 */
    seq_t sum_pow_two = 1;
    for (idx_t i = 0; i < v + 1; ++i)
        sum_pow_two *= 2;
    sum_pow_two -= 1;

    /* compute sigma(n) via prime factorisation */
    seq_t sigma = 1;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];

        /* term = (p^{e+1} - 1) / (p - 1) */
        seq_t num = seq_pow_seq(p, e + 1) - 1;
        seq_t denom = p - 1;
        seq_t term = num / denom;

        sigma *= term;
    }

    /* final result: sum of divisors not powers of 2 */
    return sigma - sum_pow_two;
}
