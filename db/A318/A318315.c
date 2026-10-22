/*
 * A318315  The 2-adic valuation of A318314.
 * https://oeis.org/A318315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = sum_{p^e||n} ( 2*e - popcount(e) if p = 2 e - popcount(e)
 *            otherwise )
 * Range:     u8 n <= 9710, u16 n <= 9710, u32 n <= 9710, u64 n <= 9710,
 *            u128 n <= 9710, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Additive arithmetic function defined on prime powers. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 (should never occur for the
       OEIS offset but keeps the function total). */
    if (n == 0)
        return 0;

    /* Factor n.  A 64‑bit integer has at most 16 distinct prime
       factors, but allocating 64 slots is more than enough. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    uint64_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* popcount of e  (number of 1’s in the binary expansion). */
        uint64_t pop = __builtin_popcountll(e);
        uint64_t term = (p == 2) ? (2 * e - pop) : (e - pop);
        result += term;
    }
    return (seq_t)result;
}
