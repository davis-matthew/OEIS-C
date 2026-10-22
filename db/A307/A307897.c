/*
 * A307897  Duplicate of A008651.
 * https://oeis.org/A307897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)= 1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1, a(7)=0,
 *            a(8)=0 a(n)= -a(n-1)+a(n-3)+a(n-4)+a(n-5)+a(n-6)-a(n-8)-a(n-9)
 *            for n>8
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed values, grows as needed */
    static seq_t *cache = NULL;
    static idx_t cap = 0;                  /* number of allocated elements */

    /* (1) ensure the cache can hold indices up to n */
    if (n >= cap) {
        idx_t old_cap = cap;
        /* always keep capacity at least 9 for the known base values */
        idx_t new_cap = (n + 1 > 9) ? (n + 1) : 9;

        if (cache == NULL) {                  /* first allocation */
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!cache) abort();              /* allocation failed */
            for (idx_t i = 0; i < new_cap; ++i)
                cache[i] = 0;                  /* initial zero values */
            /* set the known initial values */
            cache[0] = 1;
            cache[1] = 0;
            cache[2] = 0;
            cache[3] = 0;
            cache[4] = 0;
            cache[5] = 0;
            cache[6] = 1;
            cache[7] = 0;
            cache[8] = 0;
        } else {                              /* resize existing cache */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            for (idx_t i = cap; i < new_cap; ++i)
                cache[i] = 0;
        }
        cap = new_cap;

        /* (2) compute any missing terms, only from the largest needed index onward */
        idx_t start = (old_cap > 9) ? old_cap : 9;  /* recurrence needs a(i-9) */
        for (idx_t i = start; i < cap; ++i) {
            cache[i] = -cache[i-1] + cache[i-3] + cache[i-4]
                       + cache[i-5] + cache[i-6]
                       - cache[i-8] - cache[i-9];
        }
    }

    return cache[(size_t)n];
}
