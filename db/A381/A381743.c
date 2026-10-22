/*
 * A381743  The number of divisors d of n such that d*n is abundant.
 * https://oeis.org/A381743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n such that d*n is abundant
 * Range:     u8 n <= 12, u16 n <= 1748, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"

/* Recursive helper that enumerates all divisors of n.
   `pr[i]` and `ex[i]` hold the i‑th prime factor and its exponent.
   `idx` is the current prime index being processed. */
static void rec(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                size_t idx, idx_t cur, idx_t n, seq_t *res)
{
    if (idx == cnt) {
        /* cur is a divisor of n */
        unsigned __int128 prod128 = (unsigned __int128)cur * n;
        idx_t prod = (idx_t)prod128;          /* prod <= n² <= 1e8 for n ≤ 1e4 */
        seq_t sigma = sigma_of(prod);
        seq_t twice = (seq_t)2 * prod;
        if (sigma > twice) (*res)++;
        return;
    }

    uint64_t p   = pr[idx];
    uint64_t emax = ex[idx];
    idx_t mult = 1;
    for (uint64_t e = 0; e <= emax; ++e) {
        rec(pr, ex, cnt, idx + 1, cur * mult, n, res);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset starts at 1 */

    /* Factor n (sufficient size for n ≤ 10 000). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    rec(primes, exps, cnt, 0, (idx_t)1, n, &result);
    return result;
}
