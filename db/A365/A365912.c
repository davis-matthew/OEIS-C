/*
 * A365912  Expansion of e.g.f. 1 / ( 1 - Sum_{k>=0} x^(5*k+3) / (5*k+3)! ).
 * https://oeis.org/A365912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A365912)
   a(0) = 1
   a(n) = Sum_{k=0..floor((n-3)/5)} binomial(n,5*k+3) * a(n-5*k-3)   (n>=1) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) returns seq_t */

#ifdef __cplusplus
#include <new>   /* for placement new in C++ compilation */
#endif

static seq_t *cache = NULL;       /* memoised values a(0)…a(n) */
static size_t cache_alloc = 0;    /* number of elements allocated */
static size_t cache_computed = 0; /* highest index that has been computed+1 */

/* Ensure that cache has room for indices 0 … n (inclusive)  */
/* Adjust allocation size after realloc by constructing new elements if needed */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_alloc)
        return;

    /* If cache is NULL, realloc behaves as malloc */
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */

    /* Construct newly allocated slots */
    for (size_t i = cache_alloc; i < needed; ++i) {
#ifdef __cplusplus
        ::new((void*)&tmp[i]) seq_t();
#else
        tmp[i] = 0;                     /* assume POD type in C */
#endif
    }

    cache = tmp;
    cache_alloc = needed;
}

/* Compute a(n) with memoisation and dynamic programming */
seq_t a(idx_t n)
{
    if (n == 0) {
        ensure_cache(0);
        if (cache[0] == 0)               /* initialise if not already */
            cache[0] = 1;
        cache_computed = 1;               /* 0 now computed */
        return cache[0];
    }

    ensure_cache(n);

    /* Determine the first index that still needs to be computed */
    size_t start = cache_computed;
    if (start == 0) {                    /* initialisation at start */
        cache[0] = 1;
        start = 1;
    }

    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        if (i >= 3) {
            idx_t maxk = (idx_t)((i - 3) / 5);
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t sub = (idx_t)(i - (5 * k + 3));
                seq_t term = binomial((idx_t)i, 5 * k + 3);
                term *= cache[sub];
                sum += term;
            }
        }
        cache[i] = sum;
    }

    cache_computed = (size_t)n + 1;
    return cache[(size_t)n];
}
