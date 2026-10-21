/*
 * A296964  Expansion of e.g.f. (exp(x)-x)*x/(1-x).
 * https://oeis.org/A296964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A296964: Number of quasilinear weak orderings on {1,…,n}
   with exactly one maximal element.
   Recurrence:
          a(0) = 0
          a(1) = 1
          a(2) = 2
          a(n) = n * ( a(n-1) + 1 )   for n ≥ 3
*/
#include <stdlib.h>

/* memoisation table */
static seq_t *cache      = NULL; /* array a[0 … max]   */
static idx_t  cache_max  = 0;    /* highest index stored  */

/* make sure all terms up to n are computed */
static void ensure_terms(idx_t n)
{
    /* initial allocation */
    if (!cache) {
        size_t init_cap = n + 1;
        if (init_cap < 3) init_cap = 3;          /* at least indices 0,1,2 */

        cache      = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_max  = 0;                          /* currently nothing stored */

        /* initialise known base values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_max = 2;
    }

    /* extend allocation if needed */
    if (n + 1 > (size_t)(cache_max + 1)) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
    }

    /* compute new terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 2) continue; /* base values already set          */
        /* recurrence: a(i) = i * ( a(i-1) + 1 ) */
        cache[i] = (seq_t)i * (cache[i - 1] + (seq_t)1);
    }

    if (n > cache_max) cache_max = n;
}

/* entry point for the sequence */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n];
}
