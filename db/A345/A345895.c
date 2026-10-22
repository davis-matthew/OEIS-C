/*
 * A345895  a(n) = Sum_{d|n} n^sigma(d).
 * https://oeis.org/A345895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} n^sigma(d) (OEIS A345895)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/big_pow.h"

/* Recursively enumerate all divisors of n and add n^sigma(d) to *sum. */
static void _add_divs(size_t idx, idx_t cur, const uint64_t *pr,
                      const uint64_t *ex, size_t cnt,
                      idx_t n, seq_t *sum)
{
    if (idx == cnt) {
        seq_t sigma = sigma_of(cur);
        seq_t term  = big_pow((seq_t)n, sigma);
        *sum += term;
        return;
    }

    uint64_t p   = pr[idx];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        _add_divs(idx + 1, cur * val, pr, ex, cnt, n, sum);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for out‑of‑range indices */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    _add_divs(0, 1, primes, exps, cnt, n, &sum);

    return sum;
}
