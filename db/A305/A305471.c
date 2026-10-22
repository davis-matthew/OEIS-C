/*
 * A305471  a(0) = 1, a(1) = 3, a(n) = 3*n*a(n-1) - a(n-2).
 * https://oeis.org/A305471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 3, a(n) = 3*n*a(n-1) - a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, free */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    size_t idx = (size_t)n;

    if (idx >= cache_size) {
        size_t new_cap = idx + 1;
        size_t old_cap = cache_size;

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;            /* allocation failure */

        for (size_t i = 0; i < old_cap; ++i)
            new_cache[i] = cache[i];

        if (cache)
            free(cache);

        cache = new_cache;
        cache_size = new_cap;

        for (size_t i = old_cap; i < cache_size; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)3;
            else
                cache[i] = cache[i - 1] * (seq_t)i * (seq_t)3 - cache[i - 2];
        }
    }

    return cache[idx];
}
