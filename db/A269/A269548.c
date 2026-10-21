/*
 * A269548  Expansion of (-7*x^2 + 134*x + 1)/(x^3 - 99*x^2 + 99*x - 1).
 * https://oeis.org/A269548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 4, i64 n <= 9, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -1, a(1) = -233, a(2) = -22961
   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3)   (derived from the generating function
   (-7*x^2 + 134*x + 1)/(x^3 - 99*x^2 + 99*x - 1) */
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;   /* number of valid entries in cache */
static idx_t  cache_cap  = 0;   /* current allocation size (in terms) */

/* Ensure that cache[n] is computed */
static void ensure_cache_size(idx_t n)
{
    /* already have up to n */
    if (cache_size > n) return;

    /* grow allocation if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap : 3;
        /* double until large enough */
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                          (size_t)new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first three terms if this is the first call */
    if (cache_size == 0) {
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)-233;
        cache[2] = (seq_t)-22961;
        cache_size = 3;
    }

    /* compute terms from cache_size up to n (only for n >= 3) */
    if (n >= 3) {
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence applies for i >= 3; the first two values are
               already set */
            cache[i] = (seq_t)99 * cache[i-1]
                     - (seq_t)99 * cache[i-2]
                     +       cache[i-3];
        }
        cache_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
