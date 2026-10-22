/*
 * A311385  Coordination sequence Gal.6.221.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=16,
   a(5)=22, a(6)=28, a(7)=34, a(8)=38, a(9)=42, a(10)=46,
   a(11)=50, a(12)=54 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Precomputed initial values */
static const seq_t init[13] = {
    1, 4, 8, 12, 16, 22, 28, 34, 38, 42, 46, 50, 54
};

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough to hold index n */
    if ((size_t)n >= cache_size) {
        size_t new_size = (cache_size == 0) ? 1 : cache_size;
        while (new_size <= (size_t)n) {
            new_size *= 2;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – terminate the program */
            exit(1);
        }
        cache = new_cache;

        /* Compute new entries */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i <= 12) {
                cache[i] = init[i];
            } else {
                cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
