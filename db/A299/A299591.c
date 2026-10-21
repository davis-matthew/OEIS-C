/*
 * A299591  Number of n X 4 0..1 arrays with every element unequal to 0, 1 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A299591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A299591: Number of n × 4 0–1 arrays with the stated adjacency rule,
   upper‑left element zero.  
   Empirical recurrence (valid for n > 9):
         a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) - a(n-5) - a(n-6)
   Initial values (n = 1 … 9):
         5, 5, 11, 23, 53, 121, 250, 533, 1162
   The function below memoises results so that calls in any order are
   effectively O(n) with a small constant factor.  It works for any
   integer type assigned to seq_t (fixed‑width or arbitrary precision).
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed terms.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated slots (index 0 … cache_cap-1) */
    static idx_t cache_len = 0;   /* highest index that has been filled */

    if (n == 0)                     /* undefined for the sequence; return 0 */
        return 0;

    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        /* Ensure room for indices 0 … 9.  cache_cap is a raw count. */
        cache_cap = 10;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)                 /* allocation failure – unlikely in tests */
            return 0;

        /* Set all entries to 0 first to avoid garbage. */
        for (idx_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;

        /* Initial values from the OEIS entry. */
        cache[1] = 5;
        cache[2] = 5;
        cache[3] = 11;
        cache[4] = 23;
        cache[5] = 53;
        cache[6] = 121;
        cache[7] = 250;
        cache[8] = 533;
        cache[9] = 1162;

        cache_len = 9;               /* we have filled up to index 9 */
    }

    /* Grow the cache if the requested index is outside the current capacity. */
    if (n >= cache_cap) {
        /* Allocate enough for indices 0 … n. */
        cache_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
        if (!tmp)                     /* allocation failure */
            return 0;
        cache = tmp;
    }

    /* Compute missing terms up to n using the recurrence. */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* The recurrence requires terms i-1 … i-6, all already present. */
            cache[i] = cache[i-1] + cache[i-2] + (seq_t)3 * cache[i-3]
                       + cache[i-4] - cache[i-5] - cache[i-6];
        }
        cache_len = n;
    }

    return cache[n];
}
