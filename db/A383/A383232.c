/*
 * A383232  Expansion of e.g.f. f(x)^2 * log(f(x)), where f(x) = 1/(1 -
 *          5*x)^(1/5).
 * https://oeis.org/A383232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383232
   Recurrence: a(0) = 0, a(1) = 1
               a(n) = (10*n-11)*a(n-1) - (5*n-8)^2 * a(n-2)  for n > 1
   Computes the terms by dynamic programming with memoisation so that
   calls may come in any order.  All arithmetic is performed on the
   seq_t type. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed a(n) values */
static size_t cache_size = 0;    /* current number of cached entries */

/* Return a(n).  The function may be called with n in any order.  It
   lazily allocates and expands the cache as needed. */
seq_t a(idx_t n)
{
    /* Initialise cache with the first two terms if not already done. */
    if (cache == NULL) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* If the requested value is already cached, return it. */
    if ((size_t)n < cache_size) {
        return cache[(size_t)n];
    }

    /* Determine the required size of the cache. */
    size_t required = (size_t)n + 1;
    size_t old_size = cache_size;

    /* Grow the cache if necessary. */
    if (required > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;
        cache_size = required;
    }

    /* Compute new values up to n inclusively. */
    for (size_t i = old_size; i < required; ++i) {
        idx_t ii = (idx_t)i;                /* cast index to idx_t */
        seq_t term1 = ((seq_t)(10 * ii - 11)) * cache[i - 1];
        seq_t base   = ((seq_t)(5 * ii - 8));
        seq_t term2  = base * base * cache[i - 2];
        cache[i] = term1 - term2;
    }

    /* Return the requested value. */
    return cache[(size_t)n];
}
