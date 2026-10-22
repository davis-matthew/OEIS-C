/*
 * A300738  Number of minimal total dominating sets in the n-cycle graph.
 * https://oeis.org/A300738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 78, u64 n <= 157,
 *            u128 n <= 315, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A300738
   Formula:
       a(n) = a(n-3) + a(n-4) + a(n-5) + a(n-6) - a(n-8) - a(n-9)   (n > 9)
   Initial values:
       a(1)=0, a(2)=0, a(3)=3, a(4)=4, a(5)=5, a(6)=9,
       a(7)=7, a(8)=4, a(9)=12
*/

#include <stddef.h>
#include <stdlib.h>

/* Static cache for computed values (index 0 unused) */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t computed_up_to = 0;

/* Ensure the cache can hold index n and contains values up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate an initial capacity that covers at least indices 0..9 */
        cache_capacity = (size_t)((n + 1) > 10 ? (n + 1) : 10);
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        for (size_t i = 0; i < cache_capacity; ++i) cache[i] = 0;
        /* Set the known initial values */
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 3;
        cache[4] = 4;
        cache[5] = 5;
        cache[6] = 9;
        cache[7] = 7;
        cache[8] = 4;
        cache[9] = 12;
        computed_up_to = 9;
    }

    /* Expand the array if necessary */
    if ((size_t)(n + 1) > cache_capacity) {
        size_t new_cap = cache_capacity;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cache_capacity; i < new_cap; ++i) cache[i] = 0;
        cache_capacity = new_cap;
    }

    /* Compute missing values up to n */
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            if (i <= 9) {
                /* Values already initialized */
                continue;
            } else {
                seq_t val = cache[i-3] + cache[i-4] + cache[i-5] + cache[i-6];
                val -= cache[i-8];
                val -= cache[i-9];
                cache[i] = val;
            }
        }
        computed_up_to = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety for indices before the offset */
    ensure_cache(n);
    return cache[n];
}
