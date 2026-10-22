/*
 * A351620  a(1) = 1; a(n) = a(n-1) + Sum_{k=2..n} a(floor(n/k)).
 * https://oeis.org/A351620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + Sum_{k=2..n} a(floor(n/k))
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoizes results so that calls may arrive in any order. */

#include <stdlib.h>
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;          /* cache[1..] */
static idx_t  cache_capacity = 0;    /* allocated size (>=1) */
static idx_t  cache_computed = 0;    /* highest index already computed */

seq_t a(idx_t n)
{
    /* undefined for n < 1, return 0 as a safe default */
    if (n < 1U) {
        return (seq_t)0;
    }

    /* allocate or expand cache if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache) abort();            /* cannot allocate */
        cache_capacity = n + 1;
    } else if (cache_capacity <= n) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!cache) abort();            /* cannot reallocate */
        cache_capacity = n + 1;
    }

    /* initialise base case once */
    if (cache_computed == 0) {
        cache[1] = (seq_t)1;
        cache_computed = 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 2; k <= i; ++k) {
            sum += cache[i / k];
        }
        cache[i] = cache[i - 1] + sum;
    }

    cache_computed = n;
    return cache[n];
}
