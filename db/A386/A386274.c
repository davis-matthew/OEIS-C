/*
 * A386274  Expansion of 1/(1 - 49*x)^(5/7).
 * https://oeis.org/A386274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 434 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = 7 * (7*n - 2) / n * a(n-1)   for n >= 1   (equivalent to
   n*a(n) + 7*(-7*n+2)*a(n-1) = 0)        */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements allocated   */
    static idx_t max_filled = 0;  /* highest index computed, inclusive */

    /* allocate / reallocate cache if needed */
    if (cache_len <= n)
    {
        idx_t new_len = n + 1;
        if (cache == NULL)
            cache = mem_new(seq_t, new_len);
        else
            cache = mem_resize(seq_t, cache, new_len);
        cache_len = new_len;
    }

    /* initialise base case if first call */
    if (max_filled == 0 && cache_len > 0 && cache[0] == 0)
        cache[0] = (seq_t)1;          /* a(0) = 1 */

    /* compute missing values up to n */
    for (idx_t k = max_filled + 1; k <= n; ++k)
    {
        seq_t mult = (seq_t)7 *
                     (((seq_t)7 * (seq_t)k) - (seq_t)2);
        /* Use multiplication then division; for builtin types
           the intermediate product remains within bounds for all
           values that fit in seq_t. */
        cache[k] = cache[k - 1] * mult / (seq_t)k;
    }
    if (max_filled < n)
        max_filled = n;

    return cache[n];
}
