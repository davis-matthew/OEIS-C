/*
 * A242227  a(n) = (2*n-1) * a(n-1) - a(n-2), a(0) = 1, a(1) = 2.
 * https://oeis.org/A242227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A242227
   Recurrence: a(0) = 1, a(1) = 2,
   a(n) = (2*n-1) * a(n-1) - a(n-2)   for n ≥ 2
*/

#include <stdlib.h>
#include <stddef.h>
/* Use the utility provided for dynamic array allocation of seq_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Cached values --------------------------------------------- */
    static seq_t *cache       = NULL;   /* array holding computed terms */
    static size_t cache_cap   = 0;      /* current allocated capacity  */
    static idx_t  max_cached  = 0;      /* highest index that has been computed */

    /* ------------------------------------------------------------------ */
    /* First call – initialise cache with the first two terms.          */
    /* ------------------------------------------------------------------ */
    if (cache == NULL) {
        cache_cap = 2;
        cache = seq_alloc(cache_cap);          /* allocates cache_cap objects of type seq_t */
        if (cache == NULL) abort();            /* out of memory */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        max_cached = 1;
    }

    /* ------------------------------------------------------------------ */
    /* Return cached value if it is already available.                    */
    /* ------------------------------------------------------------------ */
    if (n <= max_cached) return cache[n];

    /* ------------------------------------------------------------------ */
    /* Grow cache if necessary – use seq_realloc which correctly calls    */
    /* constructors for seq_t objects.                                    */
    /* ------------------------------------------------------------------ */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* exponential growth */

        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (tmp == NULL) abort();                    /* out of memory */

        cache     = tmp;
        cache_cap = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /* Compute terms up to n using the recurrence.                       */
    /* ------------------------------------------------------------------ */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t coeff = ((seq_t)2 * (seq_t)i) - (seq_t)1;   /* 2*i-1 */
        seq_t val   = coeff * cache[i - 1] - cache[i - 2];
        cache[i]     = val;
    }

    max_cached = n;
    return cache[n];
}
