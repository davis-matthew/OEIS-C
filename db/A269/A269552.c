/*
 * A269552  Expansion of (-3*x^2 + 94*x - 3)/(x^3 - 99*x^2 + 99*x - 1).
 * https://oeis.org/A269552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269552: Expansion of (-3*x^2 + 94*x - 3)/(x^3 - 99*x^2 + 99*x - 1).
   Recurrence: a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3),  n >= 3
   Initial values: a(0)=3, a(1)=203, a(2)=19803.  Offset: 0 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* Cache of computed terms */
static idx_t cache_size = 0;         /* Number of terms stored in cache */

seq_t a(idx_t n)
{
    /* Return initial terms directly */
    if (n == 0) return 3;
    if (n == 1) return 203;
    if (n == 2) return 19803;

    /* Initialize cache with first three terms */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) { return 0; }    /* fallback on allocation failure */
        cache[0] = 3;
        cache[1] = 203;
        cache[2] = 19803;
    }

    /* Extend cache up to requested index if necessary */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) { return 0; } /* fallback on allocation failure */
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = 99*a(i-1) - 99*a(i-2) + a(i-3) */
            cache[i] = 99 * cache[i - 1] - 99 * cache[i - 2] + cache[i - 3];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
