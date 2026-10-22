/*
 * A393683  Upper (1/3,1/6) midsequence of (n^2) and ((n+2)^2); see Comments.
 * https://oeis.org/A393683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-6) - 2*a(n-7) + a(n-8), a(0) = 1,
 *            a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 12, a(5) = 17, a(6) = 23,
 *            a(7) = 30
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic cache of computed values   */
static size_t cache_size = 0;        /* current number of stored terms     */
static const seq_t init[8] = {1, 2, 4, 8, 12, 17, 23, 30};

seq_t a(idx_t n)
{
    /* If we already have the value, return it immediately. */
    if ((size_t)n < cache_size) {
        return cache[(size_t)n];
    }

    /* Need to grow the cache to include index n. */
    size_t new_cap = (size_t)n + 1;          /* new cache size */

    /* Reallocate / grow the array, handling C and C++ differences. */
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (cache != NULL) {
        for (size_t i = 0; i < cache_size; ++i) {
            tmp[i] = cache[i];
        }
        delete [] cache;
    }
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* realloc preserves existing data in C, so no copy needed. */
#endif

    cache = tmp;
    /* Initialise missing terms from the previous size up to the new capacity. */
    for (size_t i = (size_t)cache_size; i < new_cap; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        } else {
            /* Apply the recurrence: 2*a(i-1) - a(i-2) + a(i-6) - 2*a(i-7) + a(i-8) */
            seq_t term = cache[i-1] * ((seq_t)2)         /* 2*a(i-1) */
                       - cache[i-2]                      /* - a(i-2) */
                       + cache[i-6]                      /* + a(i-6) */
                       - (seq_t)2 * cache[i-7]           /* - 2*a(i-7) */
                       + cache[i-8];                     /* + a(i-8) */
            cache[i] = term;
        }
    }

    cache_size = new_cap;
    return cache[(size_t)n];
}
