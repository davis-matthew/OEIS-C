/*
 * A398204  Lower (-1/3,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) with
 *            (a(0),...,a(4)) = (0,0,3,7,12)
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    const seq_t init_vals[5] = {0, 0, 3, 7, 12};

    /* First call – initialise the cache with the five known terms. */
    if (cache == NULL) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
    }

    /* If the requested term is already in the cache, return it. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Extend the cache to contain the term at index n. */
    size_t new_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    for (size_t i = cache_len; i < new_len; ++i) {
        /* Use the recurrence to compute a(i). */
        seq_t val = cache[i - 1] + cache[i - 1];   /* 2*a(i-1) */
        val -= cache[i - 2];                       /* - a(i-2) */
        val += cache[i - 3];                       /* + a(i-3) */
        val -= cache[i - 4] + cache[i - 4];         /* - 2*a(i-4) */
        val += cache[i - 5];                       /* + a(i-5) */
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[(size_t)n];
}
