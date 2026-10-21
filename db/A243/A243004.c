/*
 * A243004  a(n) = - (a(n-1) + 1) * a(n-2) with a(1) = a(2) = 1.
 * https://oeis.org/A243004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - (a(n-1) + 1) * a(n-2) , a(1) = a(2) = 1
 * Range:     i8 n <= 8, i16 n <= 9, i32 n <= 11, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) is the first term listed for OEIS offset 1 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Static cache for memoisation (1‑based indexing). */
static seq_t *cache   = NULL;   /* array holding a(1)..a(current_max) */
static size_t cache_cap = 0;    /* allocated size (in elements)        */
static idx_t  cache_sz  = 0;    /* number of terms already computed    */

/* Ensure the cache can hold values up to n (1‑based).  */
static void ensure_capacity(idx_t n)
{
    if (n <= (idx_t)cache_sz)          /* already large enough */
        return;

    /* Determine a new capacity (at least n+1, doubled each time). */
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    /* Allocate new storage.  */
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (!new_cache)            /* allocation failed – ignore, leave cache_*! */
        return;

    /* Copy existing values.  */
    if (cache) {
        for (idx_t i = 1; i <= cache_sz; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
    }

    cache     = new_cache;
    cache_cap = new_cap;
    /* cache_sz stays unchanged – the new space is uninitialised. */
}

/* Main function returning the nth term.  */
seq_t a(idx_t n)
{
    if (n < 1)                /* out of bounds – return 0 */
        return (seq_t)0;

    ensure_capacity(n);

    /* Compute any missing terms up to n.  */
    if (cache_sz < n) {
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            if (i == 1 || i == 2)
                cache[i] = (seq_t)1;
            else
                cache[i] = -(cache[i-1] + (seq_t)1) * cache[i-2];
        }
        cache_sz = n;
    }

    return cache[n];
}
