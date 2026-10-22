/*
 * A367315  a(1) = 1, and for any n > 0, a(2*n) is the number of k's among
 *          1..n such that a(k) <= a(n), a(2*n+1) is the number of k's among
 *          1..n such that a(k) >= a(n).
 * https://oeis.org/A367315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1.
   For n > 0 :
     a(2*n)   = #{ k | 1 ≤ k ≤ n , a(k) ≤ a(n) }
     a(2*n+1) = #{ k | 1 ≤ k ≤ n , a(k) ≥ a(n) }   */

/* This implementation stores already computed values in a static cache
   to allow repeated calls with arbitrary order of n.  The cache grows
   as needed.  All arithmetic is performed in seq_t; indices are of
   type idx_t.  Only standard C99/C11 facilities (malloc, realloc,
   memset, etc.) are used, so it compiles with any of the allowed
   builds. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i = 1 .. cache_cap-1.
       cache[0] is unused. */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   cache_size = 0;   /* highest index that has been computed */

    /* Initialise cache on first entry. */
    if (cache == NULL) {
        cache_cap = 64;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure → zero */
        memset(cache, 0, cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)1;                /* a(1) = 1 */
        cache_size = 1;
    }

    /* Grow the cache if the requested index is larger than current capacity. */
    if ((size_t)n + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        while ((size_t)n + 1 > cache_cap) cache_cap <<= 1;  /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;                   /* allocation failure */
        cache = tmp;
        /* initialise the new part to zero */
        memset(cache + old_cap, 0, (cache_cap - old_cap) * sizeof(seq_t));
    }

    /* Return cached value if already computed. */
    if (n <= cache_size) return cache[n];

    /* Compute successive terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if ((i & 1u) == 0) {        /* i even: i = 2*k */
            idx_t k = i >> 1;
            seq_t thresh = cache[k];
            seq_t cnt = 0;
            for (idx_t t = 1; t <= k; ++t) {
                if (cache[t] <= thresh)
                    ++cnt;
            }
            cache[i] = cnt;
        } else {                    /* i odd: i = 2*k + 1 */
            idx_t k = (i - 1) >> 1;
            seq_t thresh = cache[k];
            seq_t cnt = 0;
            for (idx_t t = 1; t <= k; ++t) {
                if (cache[t] >= thresh)
                    ++cnt;
            }
            cache[i] = cnt;
        }
    }

    cache_size = n;
    return cache[n];
}
