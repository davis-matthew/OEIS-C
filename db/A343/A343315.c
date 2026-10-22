/*
 * A343315  a(n) is the number of nonnegative integers that can be represented
 *          in a 7-segment display by using only n segments (version A277116).
 * https://oeis.org/A343315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)+4*a(n-5)+2*a(n-6)+a(n-7) for n > 13
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=6, a(6)=6, a(7)=14,
 *            a(8)=19, a(9)=36, a(10)=67, a(11)=103, a(12)=194, a(13)=315
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t cache_max = (idx_t)(-1);  /* means not initialized */

    /* initial values */
    static const seq_t init[14] = {0, 0, 1, 1, 2, 6, 6, 14, 19,
                                   36, 67, 103, 194, 315};

    /* ensure base cache is ready */
    if (cache == NULL) {
        cache_size = 14;
        cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory: give 0 */
        for (size_t i = 0; i < 14; ++i) cache[i] = init[i];
        cache_max = 13;
    }

    /* if requested index already computed, return it */
    if (n <= cache_max) return cache[(size_t)n];

    /* grow cache to accommodate n */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* fail: give 0 */
        cache = tmp;
        cache_size = new_size;
        /* zero-initialize new region (not strictly necessary) */
        for (size_t i = cache_max + 1; i < new_size; ++i) cache[i] = 0;
    }

    /* compute values from cache_max+1 up to n */
    for (size_t i = (size_t)cache_max + 1; i <= (size_t)n; ++i) {
        /* recurrence uses indices that are guaranteed to be <= i-2 */
        cache[i] = cache[i - 2] + cache[i - 3] + cache[i - 4]
                 + 4 * cache[i - 5] + 2 * cache[i - 6] + cache[i - 7];
    }
    cache_max = (idx_t)n;
    return cache[(size_t)n];
}
