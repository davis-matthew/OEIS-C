/*
 * A353776  a(n) = Sum_{d|n} (n/d mod d).
 * https://oeis.org/A353776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d mod d)
 * Range:     u8 n <= 1079, u16 n <= 8365, u32 n <= 8365, u64 n <= 8365,
 *            u128 n <= 8365, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void dfs(const uint64_t *pr, const size_t *ex, size_t idx, size_t cnt,
                uint64_t cur, uint64_t n, seq_t *sum)
{
    if (idx == cnt) {
        seq_t d = (seq_t)cur;
        seq_t q = (seq_t)(n / cur);
        seq_t mod = q % d;
        *sum += mod;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        dfs(pr, ex, idx + 1, cnt, cur * val, n, sum);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* values below the offset are irrelevant */
    uint64_t primes[64];
    size_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t sum = 0;
    dfs(primes, exps, 0, cnt, 1, (uint64_t)n, &sum);
    return sum;
}
