/*
 * A387717  Greatest deficient divisor of n.
 * https://oeis.org/A387717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = max{ d | d divides n and sigma(d) < 2*d } (OEIS A387717)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, factor_u64, search
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/ensure_cache.h"
#include "../utils/search.h"

/* Static cache for memoisation; 0 means "not computed" (a(n) ≥ 1 for all n). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Main sequence function.  n ≥ 1.  Uses memoisation and a divisor search. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined below offset */

    /* Ensure cache space for index n. */
    ensure_cache(n, &cache, &cache_cap);

    if (cache[n - 1] != 0)                  /* already computed */
        return cache[n - 1];

    /* Factorise n.  64 slots are more than enough for n ≤ 100 000. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t best = 0;
    /* Start with divisor 1 (sigma = 1). */
    search(primes, exps, cnt, 0, 1, (seq_t)1, &best);

    cache[n - 1] = best;
    return best;
}
