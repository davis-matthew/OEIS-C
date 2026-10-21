/*
 * A269031  Number of n X 4 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 57*a(n-2) + 56*a(n-3) - 16*a(n-4) for n>5
 *            a(1)=36, a(2)=216, a(3)=2166, a(4)=18384, a(5)=146064
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure we have the base values */
    if (!cache) {
        cache_size = 5;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0; /* allocation failure, unlikely */
        cache[0] = (seq_t)36;
        cache[1] = (seq_t)216;
        cache[2] = (seq_t)2166;
        cache[3] = (seq_t)18384;
        cache[4] = (seq_t)146064;
    }
    /* If requested n is already computed, return it */
    if (n <= cache_size) {
        return cache[n-1];
    }
    /* Grow the cache to accommodate up to n */
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (size_t)n);
    if (!new_cache) return 0; /* allocation failure */
    cache = new_cache;
    /* Compute missing terms via the recurrence */
    for (idx_t i = cache_size; i < n; ++i) {
        seq_t next = 14 * cache[i-1];
        next -= 57 * cache[i-2];
        next += 56 * cache[i-3];
        next -= 16 * cache[i-4];
        cache[i] = next;
    }
    cache_size = n;
    return cache[n-1];
}
