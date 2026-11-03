/*
 * A349141  a(n) = Sum_{d|n} phi(n/d) * A348507(d), where A348507(n) =
 *          A003959(n) - n, and A003959 is fully multiplicative with a(p) =
 *          (p+1).
 * https://oeis.org/A349141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} φ(n/d) * (A003959(d)-d)
 * Range:     u8 n <= 31, u16 n <= 767, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 96 terms through n = 9322 (2026-11-01)
 * Helpers:   factor_u64, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/totient.h"

static void dfs_divs(const uint64_t *primes, const uint64_t *exps, size_t cnt,
                     size_t idx, uint64_t cur_div, seq_t cur_f,
                     idx_t n, seq_t *sum)
{
    if (idx == cnt) {
        uint64_t q = n / cur_div;
        seq_t phi_q = totient((idx_t)q);
        seq_t diff = cur_f - (seq_t)cur_div;
        *sum += phi_q * diff;
        return;
    }
    const uint64_t p = primes[idx];
    uint64_t div = cur_div;
    seq_t f = cur_f;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        dfs_divs(primes, exps, cnt, idx + 1, div, f, n, sum);
        div *= p;
        f *= (seq_t)(p + 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t sum = 0;
    dfs_divs(primes, exps, cnt, 0, 1, 1, n, &sum);
    return sum;
}
