/*
 * A299264  Partial sums of A299258.
 * https://oeis.org/A299264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A299264: Partial sums of A299258
   Recurrence:
     a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-5) - 3*a(n-6) + 3*a(n-7) - a(n-8)
   for n > 7.
   Initial values:
     a(0) = 1  a(1) = 6  a(2) = 19  a(3) = 44  a(4) = 85
     a(5) = 147 a(6) = 236 a(7) = 357 a(8) = 514
   The function memoizes results so calls in any order are efficient. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initial values for indices 0..8 */
    static const seq_t init[9] = {
        1, 6, 19, 44, 85, 147, 236, 357, 514
    };

    /* First time: allocate and populate cache with initial terms */
    if (cache_len == 0) {
        cache_len = 9;                     /* we store indices 0..8 */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* Direct return if requested n is in the initial segment */
    if (n < cache_len)
        return cache[n];

    /* Ensure cache has capacity up to n+1 */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        /* realloc can handle NULL pointer case */
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – return 0 as a sane fallback */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute new values up to n using the recurrence */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t term = 0;
        term += 3 * cache[i-1];
        term -= 3 * cache[i-2];
        term +=   cache[i-3];
        term +=   cache[i-5];
        term -= 3 * cache[i-6];
        term += 3 * cache[i-7];
        term -=   cache[i-8];
        cache[i] = term;
    }

    return cache[n];
}
