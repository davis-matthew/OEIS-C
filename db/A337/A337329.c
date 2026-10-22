/*
 * A337329  Sum of the products of all pairs of divisors of n, (d1,d2), such
 *          that d1|n, d2|n, d1|d2 and d1<d2.
 * https://oeis.org/A337329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d1|n, d2|n, d1|d2, d1<d2} d1*d2
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"

/* Recursively enumerate all divisors d of n. */
static void dfs(size_t pos, uint64_t cur_d,
                const uint64_t *primes, const uint64_t *exps, size_t cnt,
                idx_t n, seq_t *res)
{
    if (pos == cnt) {
        /* m = n / d   */
        uint64_t m = (uint64_t)n / cur_d;
        /* sum_{k|m, k>1} k = sigma(m) - 1 */
        seq_t sum_m = sigma_of((idx_t)m) - (seq_t)1;
        seq_t d_sq  = ((seq_t)cur_d) * ((seq_t)cur_d);
        *res += d_sq * sum_m;
        return;
    }
    uint64_t pow = 1;
    for (int e = 0; e <= (int)exps[pos]; ++e) {
        dfs(pos + 1, cur_d * pow, primes, exps, cnt, n, res);
        pow *= (uint64_t)primes[pos];
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)0;
    dfs(0, 1, primes, exps, cnt, n, &result);
    return result;
}
