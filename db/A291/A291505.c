/*
 * A291505  a(n) = (n!)^7 * Sum_{i=1..n} 1/i^7.
 * https://oeis.org/A291505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1 Recurrence: a(n+1) = (n^7 + (n+1)^7) * a(n) -
 *            n^14 * a(n-1) for n > 0
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow_uint.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t max_computed = 0;

static void init_cache(void) {
    if (cache == NULL) {
        /* Start with space for indices 0 and 1 */
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = 0;
        cache[1] = 1;
        max_computed = 1;
    }
}

static void ensure_capacity(idx_t n) {
    if (n < cache_cap)
        return;
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache) {
        /* Allocation failed – abort to keep behaviour deterministic */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
    /* New slots will be filled when the main loop runs */
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    init_cache();
    if (n <= max_computed)
        return cache[n];
    /* Extend cache to need up to index n */
    ensure_capacity(n);
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        idx_t m = k - 1;                         /* recurrence index */
        seq_t term1 = (pow_uint(m, 7) + pow_uint(m + 1, 7)) * cache[m];
        seq_t term2 = pow_uint(m, 14) * cache[m - 1];
        cache[k] = term1 - term2;
    }
    max_computed = n;
    return cache[n];
}
