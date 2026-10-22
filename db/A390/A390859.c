/*
 * A390859  a(n) = Sum_{k=0..n} (-1)^k * binomial(n+k+3,n-k) * Fibonacci(k+1).
 * https://oeis.org/A390859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - a(n-4) Initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=12
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;   /* dynamic cache for computed terms          */
static size_t cache_cap = 0;   /* allocated capacity of the cache           */
static size_t cache_len = 0;   /* number of initialized entries            */

/* Ensure that the cache can store indices up to n (inclusive)         */
static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = needed > 4 ? needed : 4;
    seq_t *new_cache = NULL;

#ifdef __cplusplus
    /* In C++ the class type must be constructed */
    new_cache = new seq_t[new_cap];
#else
    /* In C we can safely use calloc for zero‑initialised storage   */
    new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
#endif
    if (!new_cache)
        abort();                     /* allocation failure */

    /* Copy any existing values into the new array */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

#ifdef __cplusplus
    delete[] cache;                   /* free old storage           */
#else
    free(cache);
#endif

    cache     = new_cache;
    cache_cap = new_cap;
}

/* Compute the nth term of sequence A390859, using memoisation */
seq_t a(idx_t n)
{
    ensure_cache_capacity(n);

    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            switch ((int)i) {
            case 0: cache[0] = 1; break;
            case 1: cache[1] = 3; break;
            case 2: cache[2] = 6; break;
            case 3: cache[3] = 12; break;
            default:
                /* a(i) = 3*a(i-1) - 3*a(i-2) + 3*a(i-3) - a(i-4) */
                cache[i] = 3 * cache[i-1] - 3 * cache[i-2]
                           + 3 * cache[i-3] - cache[i-4];
            }
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
