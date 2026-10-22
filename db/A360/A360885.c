/*
 * A360885  G.f. satisfies A(x) = 1 + x * A(x * (1 + x^2)).
 * https://oeis.org/A360885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..⌊(n-1)/3⌋} binomial(n-1-2*k, k) *
 *            a(n-1-2*k)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 23, u64 n <= 39, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, ensure_cache, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"
#include "../utils/seq_mul.h"

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = (idx_t)(-1);

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* Initialise cache if not already */
    if (cache == NULL) {
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = 1;
        last_computed = 0;
    }

    /* Return cached value if already computed */
    if (n <= last_computed)
        return cache[n];

    /* Grow cache to accommodate n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute values from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t maxk = (i - 1) / 3;
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t rem = i - 1 - 2 * k;
            seq_t bin = binomial(rem, k);
            seq_t prod = seq_mul(bin, cache[rem]);  /* bin * a(rem) */
            sum += prod;
        }
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
