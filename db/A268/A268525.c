/*
 * A268525  a(n) = r*a(ceiling(n/2))+s*a(floor(n/2)) with a(1)=1 and
 *          (r,s)=(2,3).
 * https://oeis.org/A268525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(ceil(n/2)) + 3*a(floor(n/2)), a(1)=1
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_computed = 0;

/* Ensure cache has at least indices 0..n.  The element a(1) is set to 1. */
static void ensure_cache(idx_t n) {
    idx_t required = n + 1;              /* indices 0 .. n inclusive */
    if (cache_size >= required)
        return;

    if (cache == NULL) {
        cache = seq_alloc(required);
        if (!cache) abort();
        for (idx_t i = 0; i < required; ++i)
            cache[i] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = required;
        max_computed = 1;
    } else {
        seq_t *new_cache = seq_realloc(cache, cache_size, required);
        if (!new_cache) abort();
        cache = new_cache;
        for (idx_t i = cache_size; i < required; ++i)
            cache[i] = (seq_t)0;
        cache_size = required;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;           /* undefined for n==0, return 0 */

    ensure_cache(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t ceil_half = (i + 1) >> 1;   /* (i+1)/2 */
        idx_t floor_half = i >> 1;        /* i/2 */
        cache[i] = 2 * cache[ceil_half] + 3 * cache[floor_half];
    }
    max_computed = n;
    return cache[n];
}
