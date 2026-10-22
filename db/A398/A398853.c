/*
 * A398853  Upper (1/2, 1)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 3*a(n-4) + a(n-5),
   with initial values a(0)=0, a(1)=2, a(2)=7, a(3)=15, a(4)=27
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;      /* largest index computed */
    static size_t cache_cap = 0;       /* allocated capacity */

    /* initialize cache with initial five terms if empty */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, return 0 */
        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 7;
        cache[3] = 15;
        cache[4] = 27;
        cached_max = 4;
    }

    /* if requested index already computed, return it */
    if (n <= cached_max) {
        return cache[(size_t)n];
    }

    /* extend cache capacity if needed */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms from cached_max+1 up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t t0 = cache[(size_t)(i - 1)];
        seq_t t1 = cache[(size_t)(i - 2)];
        seq_t t2 = cache[(size_t)(i - 3)];
        seq_t t3 = cache[(size_t)(i - 4)];
        seq_t t4 = cache[(size_t)(i - 5)];
        cache[(size_t)i] = 3 * t0 - 4 * t1 + 4 * t2 - 3 * t3 + t4;
    }
    cached_max = n;

    return cache[(size_t)n];
}
