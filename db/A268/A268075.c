/*
 * A268075  Number of n X 4 nonnegative integer arrays with new values
 *          introduced in each row and column in sequential order starting
 *          with zero.
 * https://oeis.org/A268075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-1) - 463*a(n-2) + 2640*a(n-3) - 6700*a(n-4) +
 *            6000*a(n-5); Initial values: a(1)=1, a(2)=8, a(3)=86, a(4)=1094,
 *            a(5)=15106
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc */

#ifdef __cplusplus
/* Helpers for allocating arrays of arbitrary‑precision seq_t in C++ builds */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#endif

seq_t a(idx_t n)
{
    /* Static cache for memoisation of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_last = 0;   /* largest n that has been computed */

    if (n == 0) return (seq_t)0;   /* offset is 1, values for n<1 are undefined */

    /* Allocate/resize the cache so it can hold indices up to n */
    if (!cache || (size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 6;   /* at least 6 slots */
        if (new_cap < 6) new_cap = 6;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *tmp = (cache == NULL) ? seq_alloc(new_cap) : seq_realloc(cache, cache_cap, new_cap);
#else
        seq_t *tmp = (cache == NULL) ?
            (seq_t *)malloc(new_cap * sizeof(seq_t)) :
            (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first five terms once */
    if (cache_last == 0) {
        cache[0] = (seq_t)0;       /* unused placeholder */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)86;
        cache[4] = (seq_t)1094;
        cache[5] = (seq_t)15106;
        cache_last = 5;
    }

    if (n <= 5) return cache[n];

    /* Ensure we start computing from a safe point */
    if (cache_last < 5) cache_last = 5;

    /* Compute terms up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t term = cache[i-1] * (seq_t)36;
        term -= cache[i-2] * (seq_t)463;
        term += cache[i-3] * (seq_t)2640;
        term -= cache[i-4] * (seq_t)6700;
        term += cache[i-5] * (seq_t)6000;
        cache[i] = term;
    }
    cache_last = n;
    return cache[n];
}
