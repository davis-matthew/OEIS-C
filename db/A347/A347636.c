/*
 * A347636  Number of ways to tile an n X n square with 1 X 1 squares and
 *          (n-2) X 2 vertical or horizontal rectangles.
 * https://oeis.org/A347636
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 7*a(n-3) + a(n-4) + 3*a(n-5) Initial
 *            values: a(5)=193, a(6)=399, a(7)=783, a(8)=1601, a(9)=3283
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* values for n < 5 are undefined; return 0 */
    if (n < 5) return (seq_t)0;

    /* static cache: a[0] corresponds to a(5) */
    static seq_t *cache = NULL;
    static idx_t cache_max_n = 4;    /* last n value computed (initially none) */

    /* initialize cache with first 5 terms if necessary */
    if (cache == NULL) {
        size_t init = 5;                       /* indices 5..9 inclusive */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;    /* allocation failed */
        cache[0] = (seq_t)193;
        cache[1] = (seq_t)399;
        cache[2] = (seq_t)783;
        cache[3] = (seq_t)1601;
        cache[4] = (seq_t)3283;
        cache_max_n = 9;                       /* we have a(9) in cache */
    }

    /* if requested n already in cache, return it */
    if (n <= cache_max_n)
        return cache[n - 5];

    /* extend cache up to required n */
    size_t cur_size = cache_max_n - 5 + 1;      /* current number of stored terms */
    size_t new_size = n - 5 + 1;                /* new number of stored terms */

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) return (seq_t)0;     /* allocation failed */
    cache = new_cache;

    /* compute recurrence for terms beyond current cache */
    for (idx_t i = (idx_t)cur_size; i < (idx_t)new_size; ++i) {
        /* i >= 5 from here on, so references are safe */
        seq_t t1 = cache[i - 1];   /* a(n-1) */
        seq_t t2 = cache[i - 2];   /* a(n-2) */
        seq_t t3 = cache[i - 3];   /* a(n-3) */
        seq_t t4 = cache[i - 4];   /* a(n-4) */
        seq_t t5 = cache[i - 5];   /* a(n-5) */

        /* compute 3*t1 + t2 + t4 + 3*t5 - 7*t3 */
        seq_t sum = 3 * t1 + t2 + t4 + 3 * t5;
        cache[i] = sum - 7 * t3;
    }

    cache_max_n = n;
    return cache[n - 5];
}
