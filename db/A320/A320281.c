/*
 * A320281  Terms that are on the positive x-axis of the square spiral built
 *          with 2*k, 2*k+1, 2*k+1 for k >= 0.
 * https://oeis.org/A320281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), a(0)=0,
 *            a(1)=1, a(2)=7, a(3)=18, a(4)=35
 * Range:     u8 n <= 10, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;           /* array of a[0] .. a[cache_len-1] */
    static idx_t cache_len = 0;           /* number of values stored */
    static size_t cache_cap = 0;          /* allocated capacity */

    /* expand cache if needed */
    if (n >= cache_len) {
        size_t needed = (size_t)(n + 1);
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failure: abort (allowed in test env) */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) cache[i] = (seq_t)0;
            else if (i == 1) cache[i] = (seq_t)1;
            else if (i == 2) cache[i] = (seq_t)7;
            else if (i == 3) cache[i] = (seq_t)18;
            else if (i == 4) cache[i] = (seq_t)35;
            else {
                /* recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-3)
                                 - 2*a(n-4) + a(n-5) */
                cache[i] =
                    (seq_t)2 * cache[i-1] -
                    cache[i-2] +
                    cache[i-3] -
                    (seq_t)2 * cache[i-4] +
                    cache[i-5];
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
