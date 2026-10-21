/*
 * A293428  a(0) = 1; and for n > 0, a(n) = a(n-1) + (A008966(4n+1) -
 *          A107078(4n+3)).
 * https://oeis.org/A293428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula (recurrence): a(0) = 1; for n > 0,
   a(n) = a(n-1) + (A008966(4n+1) - A107078(4n+3))
        = a(n-1) + (is_squarefree(4n+1) + is_squarefree(4n+3) - 1)
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"   /* provides: bool is_squarefree(seq_t x) */

/* Retrieve a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache = NULL;   /* array of computed values */
    static size_t cache_len = 0;  /* number of entries actually stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* initialise cache if this is the first call */
    if (cache_cap == 0) {
        /* start with a modest capacity */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            abort();              /* failure to allocate */
        cache[0] = (seq_t)1U;      /* a(0) = 1 */
        cache_len = 1;
    }

    /* if the requested index has already been computed, return it */
    if (n < cache_len)
        return cache[n];

    /* grow the capacity if needed */
    if (cache_len > cache_cap) {          /* sanity check – should not happen */
        cache_cap = cache_len;
    }
    while (cache_len > cache_cap) {
        /* this should never happen because we grow below, but just in case */
        cache_cap <<= 1;
    }
    if (cache_len > cache_cap) {          /* again safety */
        cache_cap = cache_len;
    }
    if (cache_len > cache_cap) {
        /* nothing, defensive */
    }
    size_t required_cap = cache_len > n + 1 ? cache_len : n + 1;
    if (required_cap > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < required_cap)   /* double until sufficient */
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values from cache_len up to n (inclusive) */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* compute 4*i+1 and 4*i+3 as seq_t for the helper */
        seq_t v1 = (seq_t)(i * 4 + 1);
        seq_t v3 = (seq_t)(i * 4 + 3);
        bool sf1 = is_squarefree(v1);
        bool sf3 = is_squarefree(v3);
        seq_t delta = (seq_t)(sf1 ? 1 : 0) + (seq_t)(sf3 ? 1 : 0) - 1U;
        cache[i] = cache[i - 1] + delta;
    }

    cache_len = (size_t)n + 1;   /* now we have indexes 0..n available */

    return cache[n];
}
