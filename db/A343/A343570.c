/*
 * A343570  If n = Product (p_j^k_j) then a(n) = Product (p_j^k_j + 3), with
 *          a(1) = 1.
 * https://oeis.org/A343570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product over prime powers p^k dividing n of (p^k + 3)
 *            with a(1) = 1. This is a multiplicative function where f(p^k) =
 *            p^k + 3
 * Range:     u8 n <= 41, u16 n <= 5609, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *primes,
                                 uint64_t *exps, size_t *cnt) */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* n fits in uint64_t because we only needed values up to ~10⁴
       in the benchmark.  For larger n the behaviour is still
       defined as long as n ≤ UINT64_MAX. */
    uint64_t m = (uint64_t)n;

    /* 64 distinct prime factors are overkill for n ≤ 10⁴,
       but safe for any n that fits in uint64_t. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pk = 1;                       /* compute p^k in seq_t */
        for (uint64_t j = 0; j < exps[i]; ++j)
            pk *= (seq_t)primes[i];
        result *= pk + (seq_t)3;
    }

    return result;
}
