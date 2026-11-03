/*
 * A341620  Number of nondeficient divisors of n.
 * https://oeis.org/A341620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [sigma(d) >= 2*d]
 * Range:     u8 n <= 107, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9748 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Number of nondeficient divisors of n) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Recursive enumeration of all divisors of n.
   idx   – current prime-factor index
   cur   – product of chosen prime powers (current divisor)
   pr    – array of distinct prime factors of n
   ex    – corresponding exponents
   pf_cnt– total number of prime factors
   cnt   – accumulated count of nondeficient divisors */
static void dfs_divs(size_t idx, uint64_t cur, uint64_t *pr, uint64_t *ex,
                     size_t pf_cnt, idx_t *cnt)
{
    if (idx == pf_cnt) {
        idx_t d_idx = cur;            /* convert to idx_t for sigma_of() */
        seq_t sigma = sigma_of(d_idx);
        seq_t two_d = (seq_t)d_idx * 2ULL;
        if (sigma >= two_d)
            ++(*cnt);
        return;
    }

    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        dfs_divs(idx + 1, cur * val, pr, ex, pf_cnt, cnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    /* Factor n into primes. */
    uint64_t nn = (uint64_t)n;
    uint64_t pr[64], ex[64];
    size_t pf_cnt = 0;
    factor_u64(nn, pr, ex, &pf_cnt);

    /* Enumerate divisors and count the nondeficient ones. */
    idx_t cnt = 0;
    dfs_divs(0, 1, pr, ex, pf_cnt, &cnt);

    return (seq_t)cnt;
}
