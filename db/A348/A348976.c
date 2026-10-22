/*
 * A348976  Möbius transform of A129283, which is sum of n and its arithmetic
 *          derivative.
 * https://oeis.org/A348976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mobius(n/d) * A129283(d) (OEIS A348976)
 * Range:     u8 n <= 127, u16 n <= 9961, u32 n <= 9961, u64 n <= 9961,
 *            u128 n <= 9961, big any n
 * Helpers:   ensure_cache, rec_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"
#include "../utils/rec_mu.h"

/* Small factorisation for uint64_t numbers.  The array sizes are chosen
 * to handle all numbers that may appear in a typical test case (≤ 2^64). */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; p += (p == 2 ? 1 : 2)) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c]   = p;
            exp[c++] = e;
        }
    }
    if (n > 1) {               /* remaining prime factor */
        pr[c]   = n;
        exp[c++] = 1;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* Cache used to avoid recomputation for repeated calls */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Ensure the cache array can hold the requested index */
    ensure_cache(n, &cache, &cap);

    if (cache[n] != 0)            /* cached value is non‑zero (sequence > 0) */
        return cache[n];

    /* Factor n once – needed for the divisor enumeration */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Temporary storage for the exponents of the current divisor */
    uint64_t e_d[16] = {0};

    seq_t sum = 0;
    rec_mu(0, cnt, primes, exps, (seq_t)1, 1, &sum, e_d);

    cache[n] = sum;
    return sum;
}
