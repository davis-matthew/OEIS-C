/*
 * A347045  Smallest divisor of n with exactly half as many prime factors
 *          (counting multiplicity) as n, or 1 if there are none.
 * https://oeis.org/A347045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor of n whose bigomega equals half of
 *            bigomega(n); return 1 if no such divisor exists
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Helper: depth‑first search over all divisor exponent combinations. */
static void dfs(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                uint64_t idx, uint64_t curOmega, seq_t curVal,
                uint64_t target, seq_t *best)
{
    if (idx == cnt) return;

    seq_t power = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        uint64_t newOmega = curOmega + e;
        if (newOmega > target) break;

        seq_t newVal = curVal * power;
        if (newOmega == target) {
            if (*best == 0 || newVal < *best) *best = newVal;
        }
        dfs(pr, ex, cnt, idx + 1, newOmega, newVal, target, best);
        power = power * ((seq_t)pr[idx]);
    }
}

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 1) return 1;

    uint64_t nn = (uint64_t)n;            /* n fits in 64 bits */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    /* compute bigomega(n) */
    uint64_t bigom = 0;
    for (size_t i = 0; i < cnt; ++i) bigom += exps[i];

    if (bigom & 1) return 1;              /* odd → no divisor */

    uint64_t target = bigom >> 1;         /* half of bigomega */
    if (target == 0) return 1;            /* n == 1 or odd but handled above */

    seq_t best = 0;                       /* 0 means not found yet */
    dfs(primes, exps, cnt, 0, 0, 1, target, &best);

    return best ? best : 1;
}
