/*
 * A342617  a(n) = Sum_{d|n} phi(d)^(n-d+1).
 * https://oeis.org/A342617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)^(n-d+1)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 19, u64 n <= 32, u128 n <= 50,
 *            big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/phi.h"

static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1)
            res = res * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return res;
}

static void dfs(idx_t n,
                size_t pos,
                seq_t cur,
                const uint64_t *primes,
                const uint64_t *exps,
                size_t tot,
                seq_t *sum)
{
    if (pos == tot)
    {
        idx_t d = (idx_t)cur;
        seq_t phi_d = phi(d);
        idx_t exp = n - d + 1;
        seq_t term = power_seq(phi_d, exp);
        *sum += term;
        return;
    }
    uint64_t p = primes[pos];
    seq_t mult = 1;
    for (uint64_t e = 0; e <= exps[pos]; ++e)
    {
        dfs(n, pos + 1, cur * mult, primes, exps, tot, sum);
        mult *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    /* factor n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    dfs(n, 0, 1, primes, exps, cnt, &result);
    return result;
}
