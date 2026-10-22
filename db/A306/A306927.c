/*
 * A306927  a(n) = A001615(n) - n.
 * https://oeis.org/A306927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n} (mu(n/d)^2 * d) i.e. sum of proper
 *            divisors d of n such that n/d is square‑free
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* test if m is squarefree by full factorisation */
static bool is_squarefree_u64(uint64_t m)
{
    if (m == 1) return true;
    /* at most 64 prime factors for 64‑bit numbers */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);   /* cnt elements filled */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 1) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t pair = n / d;
            if (d < n && is_squarefree_u64(pair)) {
                sum += (seq_t)d;
            }
            if (pair != d && pair < n && is_squarefree_u64(d)) {
                sum += (seq_t)pair;
            }
        }
    }
    return sum;
}
