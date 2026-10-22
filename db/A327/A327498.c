/*
 * A327498  Maximum divisor of n whose prime multiplicities are distinct
 *          (A130091).
 * https://oeis.org/A327498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Finds the largest divisor of n whose prime multiplicities are distinct
 *  (OEIS A327498).  Brute–force over all choices of exponents ≤ those
 *  in n, keeping them distinct by tracking used exponent values in a
 *  64‑bit bitmask.  Works for arbitrary‑precision seq_t and 64‑bit build.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursive helper: try all exponents for prime at position idx */
static void dfs_div(int idx, seq_t curr, uint64_t used,
                    const uint64_t *primes, const uint64_t *exps,
                    int k, seq_t *best)
{
    if (idx == k) {
        if (curr > *best) *best = curr;
        return;
    }

    /* Current prime */
    seq_t pow = (seq_t)1;  /* p^0 */

    /* Option: exponent 0 (skip prime) */
    dfs_div(idx + 1, curr, used,
            primes, exps, k, best);

    /* Options: exponents 1 .. exps[idx] */
    for (uint64_t e = 1; e <= exps[idx]; ++e) {
        pow *= (seq_t)primes[idx];           /* p^e */
        uint64_t bit = 1ULL << e;            /* mark exponent e as used */
        if ((used & bit) == 0) {
            dfs_div(idx + 1, curr * pow, used | bit,
                    primes, exps, k, best);
        }
    }
}

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t n64 = (uint64_t)n;
    factor_u64(n64, primes, exps, &cnt);

    if (cnt == 0) return (seq_t)1;  /* n = 1 */

    seq_t best = (seq_t)1;
    dfs_div(0, (seq_t)1, 0ULL,
            primes, exps, (int)cnt, &best);

    return best;
}
