/*
 * A368634  Expansion of g.f. A(x) satisfying A(x) = 1 + x*(3*A(x)^2 -
 *          A(-x)^2)/2.
 * https://oeis.org/A368634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A368634
 * Recurrence derived from the generating function:
 *   a(0) = 1
 *   For n >= 1 :
 *     if n is odd  -> a(n) =  Σ_{i=0}^{n-1} a(i)*a(n-1-i)
 *     if n is even -> a(n) = 2 * Σ_{i=0}^{n-1} a(i)*a(n-1-i)
 *   That is, a(n) = (1 + (n % 2)) * Σ_{i=0}^{n-1} a(i)*a(n-1-i)
 *
 * The function below computes the sequence on demand, caching results so
 * that subsequent calls are fast.  The implementation uses only integer
 * arithmetic; no floating point or external libraries are required.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache to store previously computed values */
static seq_t *cache = NULL;          /* array of a(i) */
static size_t cache_cap = 0;         /* allocated size of cache   */
static idx_t   computed_max = 0;     /* largest n for which a(n) is in cache (>=0) */

/*
 * Public function required by the test harness
 */
seq_t a(idx_t n)
{
    /* Handle the base case */
    if (n == 0) {
        /* Ensure at least one element in cache for a(0) */
        if (cache == NULL || cache_cap == 0) {
            cache = (seq_t *)calloc(1, sizeof(seq_t));
            cache_cap = 1;
        }
        /* a(0) = 1 */
        cache[0] = (seq_t)1;
        computed_max = 0;
        return cache[0];
    }

    /* Ensure cache large enough to hold up to a(n) */
    if (cache == NULL) {
        /* start with a small capacity, will grow as needed */
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure: abort, as the function cannot proceed */
            abort();
        }
    } else if ((size_t)n + 1 > cache_cap) {
        /* grow capacity (e.g., double it) */
        size_t new_cap = (size_t)n + 1;
        /* ensure some room for future growth */
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        /* zero the new part of the array */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the requested term is already computed, return it */
    if (n <= computed_max) {
        return cache[n];
    }

    /* Compute missing terms sequentially */
    for (idx_t m = computed_max + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        /* convolution sum Σ_{i=0}^{m-1} a[i] * a[m-1-i] */
        for (idx_t i = 0; i < m; ++i) {
            seq_t prod = cache[i] * cache[m - 1 - i];
            sum = sum + prod;
        }
        if ((m & 1) == 0) {
            /* even index: multiply by 2 */
            sum = sum * (seq_t)2;
        }
        cache[m] = sum;
    }

    computed_max = n;
    return cache[n];
}
