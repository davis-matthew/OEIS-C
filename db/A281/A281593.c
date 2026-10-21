/*
 * A281593  a(n) = b(n) - Sum_{j=0..n-1} b(j) with b(n) = binomial(2*n, n).
 * https://oeis.org/A281593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) - sum_{j=0}^{n-1} binomial(2*j, j)
 *            Recurrence used: a(0) = 1, a(n+1) = a(n) + 2*n*Catalan(n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/catalan.h"

/* Static cache: a_cache holds computed a(n)  (index 0 .. cached_max-1)
   a_cache_capacity is the allocated size.  a_cache_max is the highest
   computed index plus one. */
static seq_t *a_cache = NULL;
static idx_t  a_cache_capacity = 0;
static idx_t  a_cache_max = 0;

/* Ensure the cache can hold indices up to n.  The capacity is doubled
   until it is sufficient. */
static void
ensure_capacity(idx_t n)
{
    if (a_cache_capacity > n)
        return;

    idx_t  new_cap = a_cache_capacity ? a_cache_capacity : 1;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;                     /* on allocation failure, caller will
                                       get the old cache contents */

    if (a_cache) {
        memcpy(new_cache, a_cache, a_cache_max * sizeof(seq_t));
        free(a_cache);
    }
    a_cache = new_cache;
    a_cache_capacity = new_cap;
}

/* Main sequence function.  It memoises values, so subsequent calls
   are O(1) for already computed indices. */
seq_t
a(idx_t n)
{
    /* Initialise cache with a(0) if not yet done */
    if (!a_cache) {
        a_cache_capacity = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!a_cache)
            return (seq_t)0;          /* memory allocation failed */
        a_cache[0] = (seq_t)1;        /* a(0) = 1 */
        a_cache_max = 1;
    }

    /* Compute up to n if necessary */
    if (n >= a_cache_max) {
        ensure_capacity(n);
        for (idx_t i = a_cache_max; i <= n; ++i) {
            /* a(i) = a(i-1) + 2*(i-1)*Catalan(i-1) */
            seq_t term = catalan(i-1) *
                         ((seq_t)(2 * (i-1)));   /* 2*(i-1) converted to seq_t */
            a_cache[i] = a_cache[i-1] + term;
        }
        a_cache_max = n + 1;
    }

    return a_cache[n];
}
