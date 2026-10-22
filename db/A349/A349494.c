/*
 * A349494  a(n) is the maximum of A000120(k)*A000120(n/k) for divisors k of
 *          n.
 * https://oeis.org/A349494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max_{d|n} popcount(d)*popcount(n/d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void enumerate_divisors(size_t idx, uint64_t cur, uint64_t n,
                               const uint64_t *pr, const uint64_t *ex,
                               size_t pcnt, seq_t *max_val)
{
    if (idx == pcnt) {
        seq_t pc_cur   = (seq_t)__builtin_popcountll(cur);
        seq_t pc_other = (seq_t)__builtin_popcountll(n / cur);
        seq_t prod     = pc_cur * pc_other;
        if (prod > *max_val)
            *max_val = prod;
        return;
    }
    uint64_t p   = pr[idx];
    uint64_t exp = ex[idx];
    uint64_t pow = 1;
    for (size_t e = 0; e <= exp; ++e) {
        enumerate_divisors(idx + 1, cur * pow, n, pr, ex, pcnt, max_val);
        if (e < exp) {
            if (pow > n / p)  /* avoid overflow */
                break;
            pow *= p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* safety: a(0) not used */
    uint64_t nn = (uint64_t)n;
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(nn, primes, exps, &pcnt);
    seq_t max_val = 0;
    enumerate_divisors(0, 1ULL, nn, primes, exps, pcnt, &max_val);
    return max_val;
}
