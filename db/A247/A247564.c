/*
 * A247564  a(n) = 3*a(n-2) - 4*a(n-4) with a(0) = 2, a(1) = 1, a(2) = 3, a(3)
 *          = 1.
 * https://oeis.org/A247564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-2) - 4*a(n-4), a(0)=2, a(1)=1, a(2)=3, a(3)=1
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 61, i64 n <= 127,
 *            i128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;     /* array of terms */
    static size_t cache_size = 0;   /* allocated capacity */
    static size_t cache_len   = 0;   /* number of terms stored */

    /* enlarge cache if needed (C++ needs explicit cast) */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size ? cache_size : 4;
        while (new_size <= (size_t)n + 1) {
            new_size *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise the first four values on first use */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)1;
        cache_len = 4;
    }

    /* extend cache up to index n */
    while (cache_len <= (size_t)n) {
        cache[cache_len] =
            (seq_t)3 * cache[cache_len - 2] - (seq_t)4 * cache[cache_len - 4];
        ++cache_len;
    }

    return cache[n];
}
