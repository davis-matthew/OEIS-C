/*
 * A351348  Dirichlet g.f.: Product_{p prime} (1 + 2*p^(-s)) / (1 - p^(-s) -
 *          p^(-2*s)).
 * https://oeis.org/A351348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} Lucas(e+1) (Lucas numbers: L0 = 2, L1 = 1, Lk =
 *            Lk-1 + Lk-2)
 * Range:     u8 n <= 959, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/ensure_cache.h"

static seq_t lucas(idx_t k)
{
    if (k == 0) return (seq_t)2;
    if (k == 1) return (seq_t)1;
    seq_t Lprev = (seq_t)2; /* L0 */
    seq_t L = (seq_t)1;      /* L1 */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t tmp = L + Lprev;
        Lprev = L;
        L = tmp;
    }
    return L;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but guard */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure cache can hold index n */
    if (n >= cache_cap) {
        ensure_cache(n, &cache, &cache_cap);
    }

    if (cache[n] != 0) {
        return cache[n];
    }

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)exps[i];
        /* Lucas(e+1) */
        seq_t L = lucas(e + 1);
        result *= L;
    }

    cache[n] = result;
    return result;
}
