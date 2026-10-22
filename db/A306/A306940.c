/*
 * A306940  Expansion of 1/((1 - x)^6 + x^6).
 * https://oeis.org/A306940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 13, i32 n <= 29, i64 n <= 63, i128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * OEIS A306940: Expansion of 1/((1 - x)^6 + x^6)
 * Recurrence: a(0)=1, a(1)=6, a(2)=21, a(3)=56, a(4)=126, a(5)=252
 *              a(n)=6*a(n-1)-15*a(n-2)+20*a(n-3)-15*a(n-4)+6*a(n-5)-2*a(n-6)  (n>5)
 */
#include <stdlib.h>

/* Return the n‑th term of sequence A306940.  */
seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;   /* array of a(0)…a(current_max)   */
    static size_t  cache_size   = 0;   /* number of values stored       */
    static size_t  cache_cap    = 0;   /* allocated size of the array   */

    /* initialize cache when called the first time */
    if (cache == NULL) {
        /* preallocate a moderate number of slots to reduce reallocs */
        cache_cap   = 16;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)             /* allocation failed – unexpected */
            exit(1);

        /* seed the first six terms */
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 21;
        cache[3] = 56;
        cache[4] = 126;
        cache[5] = 252;
        cache_size = 6;         /* we have values 0 .. 5 stored */
    }

    /* if the requested term is already cached, return it immediately */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* enlarge the cache array if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap < needed ? needed : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {            /* realloc failed – unexpected */
            free(cache);
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
        /* Note: newly allocated slots will be overwritten when we compute them */
    }

    /* compute all missing terms up to n, using the recurrence */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* caching the current window of six previous values */
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        seq_t t6 = cache[i - 6];

        cache[i] = (seq_t)6 * t1
                 - (seq_t)15 * t2
                 + (seq_t)20 * t3
                 - (seq_t)15 * t4
                 + (seq_t)6  * t5
                 - (seq_t)2  * t6;
    }

    cache_size = (size_t)n + 1;   /* update the number of stored values */
    return cache[(size_t)n];
}
