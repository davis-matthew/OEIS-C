/*
 * A371883  a(n) is the number of divisors d of n such that d^n mod n = d.
 * https://oeis.org/A371883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d^n mod n = d
 * Range:     u8 n <= 2649, u16 n <= 9828, u32 n <= 9828, u64 n <= 9828,
 *            u128 n <= 9828, big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

static void dfs_count(size_t pos, seq_t current, const uint64_t *pr, const uint64_t *ex,
                      size_t cnt, idx_t n, seq_t *out)
{
    if (pos == cnt) {
        seq_t val = mod_pow(current, (uint64_t)n, (seq_t)n);
        if (val == current) {
            (*out)++;
        }
        return;
    }
    seq_t p = 1;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        dfs_count(pos + 1, current * p, pr, ex, cnt, n, out);
        p = p * pr[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 1) { /* 1 has divisor 1, but 1^1 mod 1 = 0 */
        return (seq_t)0;
    }

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    seq_t count = 0;
    dfs_count(0, (seq_t)1, primes, exps, pf_cnt, n, &count);
    return count;
}
