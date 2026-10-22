/*
 * A313929  Coordination sequence Gal.6.334.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=5, a(2)=10, a(3)=16, a(4)=22, a(5)=28,
   a(6)=34, a(7)=40, a(8)=46, a(9)=52, a(10)=57, a(11)=62, a(12)=67,
   a(n)=a(n-1)+a(n-11)-a(n-12) for n>12. */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Initial values for 0..12 */
static const seq_t base_vals[13] = {
    1, 5, 10, 16, 22, 28, 34, 40, 46, 52, 57, 62, 67
};

seq_t a(idx_t n)
{
    /* If n is within the known initial segment, return directly. */
    if (n < 13)
        return base_vals[n];

    /* Lazy allocation / reallocation of the dynamic cache. */
    if (cache == NULL) {
        /* Allocate for at least 13 elements. */
        cache = (seq_t *)malloc(13 * sizeof(seq_t));
        if (!cache) return 0;          /* Out of memory: undefined behaviour. */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = base_vals[i];
        cache_size = 13;
    }

    /* Extend cache if needed. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!ptr) return 0;                /* Out of memory. */
        cache = ptr;

        /* Compute all new entries up to n. */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i <= 12) {
                cache[i] = base_vals[i];
            } else {
                cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
