/*
 * A398197  Upper (2/5,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7), with
 *            (a(0),…,a(6)) = (0,2,6,12,20,31,45)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistently store the computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialisation: create space for the first 7 terms. */
    if (cache == NULL) {
        cache_size = 7;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unsafe, ignore for this context */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)20;
        cache[5] = (seq_t)31;
        cache[6] = (seq_t)45;
    }

    /* Convert idx_t to a size_t for array indexing. */
    size_t nsize = (size_t)n;

    /* Extend the cache if required. */
    if (nsize >= cache_size) {
        size_t new_size = nsize + 1;
        if (new_size < 7) new_size = 7;                     /* safety */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                         /* unsafe, ignore */
        cache = tmp;

        /* Compute missing terms using the recurrence. */
        for (size_t i = cache_size; i <= nsize; ++i) {
            cache[i] = (seq_t)2 * cache[i-1]
                       - cache[i-2]
                       + cache[i-5]
                       - (seq_t)2 * cache[i-6]
                       + cache[i-7];
        }
        cache_size = new_size;
    }

    return cache[nsize];
}
