/*
 * A291014  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - S^3)^2.
 * https://oeis.org/A291014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A291014 – has recurrence
   a(n) = 6*a(n‑1) – 15*a(n‑2) + 22*a(n‑3) – 21*a(n‑4) + 12*a(n‑5) – 4*a(n‑6)
   for n ≥ 7,
   initial values:
   a(0)=0, a(1)=0, a(2)=2, a(3)=6, a(4)=12, a(5)=23, a(6)=48 */
#ifndef A291014_SENTRY
#define A291014_SENTRY
#include <stdlib.h>   /* malloc, free */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

static seq_t *cache = NULL;   /* memoisation buffer */
static idx_t  cache_size = 0; /* number of entries stored (0 if none) */

seq_t a(idx_t n)
{
    /* initial terms – literals work for both built‑in and big‑int types */
    static const seq_t init[7] = {0, 0, 2, 6, 12, 23, 48};

    /* if we already computed up to n, just return the cached value */
    if (n < cache_size) {
        return cache[n];
    }

    /* need to compute from the largest uncomputed index up to n */
    const idx_t needed = n + 1;          /* number of entries required */
    const idx_t old_size = cache_size;   /* keep the old size before reallocation */

    if (needed > cache_size) {
        /* resize the cache buffer */
        size_t new_bytes = (size_t)needed * sizeof(seq_t);
        seq_t *tmp = NULL;

#if defined(__cplusplus)
        tmp = new seq_t[needed];
#else
        tmp = (seq_t *)malloc(new_bytes);
        if (tmp == NULL) return (seq_t)0; /* allocation failure: undefined behaviour not specified */
#endif

        /* copy previously computed values if any */
        if (cache) {
            for (idx_t i = 0; i < old_size; ++i) {
                tmp[i] = cache[i];
            }
#if defined(__cplusplus)
            delete[] cache;
#else
            free(cache);
#endif
        }

        cache = tmp;
        cache_size = needed;
    }

    /* compute the missing values */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 7) {
            cache[i] = init[i];
        } else {
            cache[i] =  (seq_t)(  6 * cache[i-1]
                               - 15 * cache[i-2]
                               + 22 * cache[i-3]
                               - 21 * cache[i-4]
                               + 12 * cache[i-5]
                               -  4 * cache[i-6] );
        }
    }

    return cache[n];
}
#endif /* A291014_SENTRY */
