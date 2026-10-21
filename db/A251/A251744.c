/*
 * A251744  8-step Fibonacci sequence starting with 0,0,1,0,0,0,0,0.
 * https://oeis.org/A251744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+8) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)+a(n+7);
 *            a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 0, a(4) = 0, a(5) = 0, a(6)
 *            = 0, a(7) = 0
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;
    static const seq_t init[8] = { (seq_t)0, (seq_t)0, (seq_t)1,
                                   (seq_t)0, (seq_t)0, (seq_t)0,
                                   (seq_t)0, (seq_t)0 };

    /* Ensure the cache is large enough to hold up to index n */
    if (n >= cache_size)
    {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (cache)
            {
                for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
                free(cache);
            }
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Compute missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            seq_t value;
            if (i < 8)
                value = init[i];
            else
            {
                seq_t sum = (seq_t)0;
                for (idx_t k = i - 8; k < i; ++k) sum += cache[k];
                value = sum;
            }
            cache[i] = value;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
