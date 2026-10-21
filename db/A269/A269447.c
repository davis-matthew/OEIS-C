/*
 * A269447  The first of 23 consecutive positive integers the sum of the
 *          squares of which is a square.
 * https://oeis.org/A269447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 48*a(n-2) - a(n-4) + 506  for n > 5
 * Initial values: a(1)=7, a(2)=17, a(3)=881, a(4)=1351, a(5)=42787
 *
 * This uses a simple recurrence with O(n) time and O(n) memory.
 * Cached values allow repeated accesses in any order.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;     /* array index [0 .. cap], 1-based usage */
static idx_t  cache_cap  = 0;        /* allocated capacity (excluding index 0) */
static idx_t  cache_filled = 0;      /* highest index that has been computed */

/* Ensure the cache can hold values up to n and compute missing terms */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;

    /* Allocate or grow the cache if necessary */
    if (!cache || n > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
        while (new_cap < n)
            new_cap <<= 1;                /* double until large enough */

        /* Allocate new array */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1));
        if (!new_cache)
            return;                       /* failure: caller will not get value */

        /* Copy existing data */
        if (cache) {
            for (idx_t i = 1; i <= cache_filled; ++i)
                new_cache[i] = cache[i];
            free(cache);                  /* free old array */
        }
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values using the recurrence */
    if (cache_filled < n) {
        for (idx_t i = cache_filled + 1; i <= n; ++i) {
            if (i == 1)
                cache[1] = 7;
            else if (i == 2)
                cache[2] = 17;
            else if (i == 3)
                cache[3] = 881;
            else if (i == 4)
                cache[4] = 1351;
            else if (i == 5)
                cache[5] = 42787;
            else
                cache[i] = (seq_t)48 * cache[i - 2] - cache[i - 4] + (seq_t)506;
        }
        cache_filled = n;
    }
}

/* Public sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined below offset 1, harmless value */

    ensure_cache(n);
    return cache[n];
}
