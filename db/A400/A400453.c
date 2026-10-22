/*
 * A400453  The sum of divisors d of n such that gcd(d, n/d) is a powerful
 *          number (A001694).
 * https://oeis.org/A400453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sum of all divisors d of n for which gcd(d, n/d) is a
 *            powerful number (every prime factor exponent ≥ 2)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void dfs(size_t idx, seq_t cur, const uint64_t *primes,
                const size_t *exps, size_t cnt, seq_t *sum)
{
    if (idx == cnt) {
        *sum += cur;
        return;
    }
    uint64_t p = primes[idx];
    size_t e = exps[idx];
    for (size_t k = 0; k <= e; ++k) {
        if (!(k == 0 || k == e || (k >= 2 && e - k >= 2)))
            continue;                      /* bad exponent for powerful gcd */
        seq_t next = cur;
        for (size_t i = 0; i < k; ++i)
            next *= p;                     /* multiply by p^k */
        dfs(idx + 1, next, primes, exps, cnt, sum);
    }
}

seq_t a(idx_t n)
{
    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs(0, (seq_t)1, primes, exps, cnt, &sum);
    return sum;
}
