/*
 * A304326  Number of ways to write n as a product of a number that is not a
 *          perfect power and a squarefree number.
 * https://oeis.org/A304326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered factorizations n = u*v with u not a
 *            perfect power (1 is excluded) and v squarefree
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static inline int gcd_int_local(int a, int b)
{
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a > 0 ? a : -a;
}

static void dfs(const uint64_t primes[], const uint64_t exps[],
                size_t idx, size_t cnt, uint64_t cur,
                uint64_t e[], seq_t *res)
{
    if (idx == cnt) {
        if (cur == 1) return;                 /* skip u = 1 (perfect power) */

        int g = 0;
        for (size_t i = 0; i < cnt; ++i) {
            int ei = (int)e[i];
            g = (g == 0) ? ei : gcd_int_local(g, ei);
        }
        if (g >= 2) return;                   /* u is a perfect power */

        for (size_t i = 0; i < cnt; ++i) {
            if (exps[i] - e[i] > 1) return;  /* v is not squarefree */
        }
        (*res)++;
        return;
    }

    uint64_t cur_pow = cur;
    for (uint64_t val = 0; val <= exps[idx]; ++val) {
        e[idx] = val;
        dfs(primes, exps, idx + 1, cnt, cur_pow, e, res);
        cur_pow *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n64, primes, exps, &cnt);

    uint64_t e[64] = {0};
    seq_t ans = 0;
    dfs(primes, exps, 0, cnt, 1ULL, e, &ans);
    return ans;
}
