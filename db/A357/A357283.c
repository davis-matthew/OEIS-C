/*
 * A357283  a(n) = number of subsets S of {1,2,...,n} having more than 1
 *          element such that (sum of least two elements of S) < max(S).
 * https://oeis.org/A357283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 6*a(n-3) + 10*a(n-4) - 4*a(n-5)
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=8
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free, NULL, memcpy */
#include <string.h>   /* memcpy */

/* static state for memoisation */
static seq_t *cache     = NULL;       /* array of cached values   */
static size_t cache_sz  = 0;          /* capacity of cache       */

/* keep the greatest index that has been computed so far
   a value of -1 means that nothing has been computed yet   */
static long long last_computed = -1;

/* ------------------------------------------------------------------ */
/* helper: grow cache to at least 'need' elements (0‑based index).    */
/* ------------------------------------------------------------------ */
static void grow_cache(size_t need)
{
    if (cache_sz > need) return;            /* already enough */

    size_t new_sz = need + 1;
#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_sz];
#else
    seq_t *new_cache = (seq_t*)malloc(new_sz * sizeof(seq_t));
#endif
    if (!new_cache) return;                 /* allocation failed */

    /* copy existing values if any */
    if (cache) {
#if defined(__cplusplus)
        for (size_t i = 0; i < cache_sz; ++i) new_cache[i] = cache[i];
        delete [] cache;
#else
        memcpy(new_cache, cache, cache_sz * sizeof(seq_t));
        free(cache);
#endif
    }

    /* zero any newly allocated part – guarantees determinism */
    if (new_sz > cache_sz) {
        memset(&new_cache[cache_sz], 0, (new_sz - cache_sz) * sizeof(seq_t));
    }

    cache = new_cache;
    cache_sz = new_sz;
}

/* ------------------------------------------------------------------ */
/* Sequence function                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure cache can hold index n */
    if (n >= cache_sz) {
        grow_cache((size_t)n);
        if (n >= cache_sz) return (seq_t)0;   /* allocation still failed */
    }

    /* Already computed? */
    if (last_computed >= 0 && (size_t)n <= (size_t)last_computed) {
        return cache[(size_t)n];
    }

    /* Compute from the first missing index up to n */
    size_t start = (last_computed >= 0) ? (size_t)last_computed + 1 : (size_t)0;

    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)0;
        } else if (i == 4) {
            cache[i] = (seq_t)2;
        } else if (i == 5) {
            cache[i] = (seq_t)8;
        } else {
            /* recurrence: a(i) = 4*a(i-1) - 3*a(i-2) - 6*a(i-3)
               + 10*a(i-4) - 4*a(i-5)          */
            seq_t t = 4 * cache[i-1];
            t -= 3 * cache[i-2];
            t -= 6 * cache[i-3];
            t += 10 * cache[i-4];
            t -= 4 * cache[i-5];
            cache[i] = t;
        }
    }

    last_computed = (long long)n;
    return cache[(size_t)n];
}
