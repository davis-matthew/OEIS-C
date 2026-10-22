/*
 * A318031  Number of nX2 0..1 arrays with every element unequal to 0, 1, 3 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 4*a(n-3) + a(n-5) - a(n-6)
   Initial values: a(1)=2, a(2)=3, a(3)=6, a(4)=11, a(5)=22, a(6)=46 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle small n without extra allocation */
    if (n <= 6) {
        static const seq_t base[] = {0, 2, 3, 6, 11, 22, 46};
        return base[n];
    }

    /* Cache for previously computed values (1‑based indexing) */
    static seq_t *cache = NULL;      /* cache[1] = a(1) */
    static idx_t cached_n = 0;       /* largest n that has been computed */

    /* Grow or create the cache if needed */
    if (cache == NULL || cached_n < n) {
        idx_t new_cap = n + 1;                  /* room for indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* allocation failed */
        cache = tmp;

        /* Initialize the first six terms if this is the first allocation or they were missing */
        if (cached_n < 6) {
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)3;
            cache[3] = (seq_t)6;
            cache[4] = (seq_t)11;
            cache[5] = (seq_t)22;
            cache[6] = (seq_t)46;
        }

        /* Compute terms from the first missing one up to n */
        idx_t start = (cached_n < 6 ? 7 : cached_n + 1);
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = cache[i-1]
                       + (seq_t)4 * cache[i-3]
                       + cache[i-5]
                       - cache[i-6];
        }

        cached_n = n;
    }

    return cache[n];
}
