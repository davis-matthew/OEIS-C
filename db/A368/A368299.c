/*
 * A368299  a(n) is the number of permutations pi of [n] that avoid {231,321}
 *          so that pi^4 avoids 132.
 * https://oeis.org/A368299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + a(n-1) + a(n-2) + a(n-4) with initial values a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n) {
    /* static cache to memoise previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (in elements) */
    static size_t cache_size = 0;  /* number of values currently stored (next free index) */

    /* initialize cache with the first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache_size = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();               /* memory allocation failed */
        cache[0] = (seq_t)0;               /* a(0) */
        cache[1] = (seq_t)1;               /* a(1) */
        cache[2] = (seq_t)2;               /* a(2) */
        cache[3] = (seq_t)4;               /* a(3) */
    }

    /* handle requests below the current cache size */
    if ((size_t)n < cache_size) {
        return cache[(size_t)n];
    }

    /* enlarge cache if necessary */
    size_t needed = (size_t)n + 1;         /* we need space up to index n inclusive */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)0;
        } else if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)2;
        } else if (i == 3) {
            val = (seq_t)4;
        } else {
            /* recurrence: a(i) = 1 + a(i-1) + a(i-2) + a(i-4) */
            val = (seq_t)1;
            val += cache[i - 1];
            val += cache[i - 2];
            val += cache[i - 4];
        }
        cache[i] = val;
    }

    cache_size = needed;
    return cache[(size_t)n];
}
