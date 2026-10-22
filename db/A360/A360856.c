/*
 * A360856  a(n) = [x^n](1/2)*(1 + (2*x + 1)/sqrt(1 - 8*x^2*(x + 1))).
 * https://oeis.org/A360856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = [x^n](1/2)*(1 + (2*x + 1)/sqrt(1 - 8*x^2*(x + 1)))
 *   Recurrence (for n ≥ 4):
 *     a(n) = (4*(2*n^2 - 11*n + 15)*a(n-3)
 *             + 4*(2*n^2 - 9*n + 9)*a(n-2)
 *             + 2*(n-3)*a(n-1)) / (n^2 - 3*n)
 *   Initial values:
 *     a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 6
 */

#ifndef __cplusplus
#include <stdlib.h>
#include <stddef.h>
#else
#include <cstdlib>
#include <cstddef>
#endif

/* static storage for memoisation */
static seq_t* cache = NULL;
static idx_t  cache_len = 0;   /* number of values stored in cache */
static size_t cache_cap = 0;   /* capacity of cache */

/* reallocates cache to new_cap (in C++ uses new/delete, in C uses realloc) */
static seq_t* grow_cache(size_t new_cap)
{
#ifdef __cplusplus
    seq_t* newp = new seq_t[new_cap];
    if (cache && cache_len > 0) {
        for (size_t i = 0; i < cache_len; ++i) newp[i] = cache[i];
    }
    delete[] cache;
    return newp;
#else
    return (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Ensure cache has enough capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 8)            new_cap = 8;
        if (new_cap < cache_cap*2)  new_cap = cache_cap*2;

        seq_t* tmp = grow_cache(new_cap);
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)      cache[i] = (seq_t)1;
        else if (i == 2)      cache[i] = (seq_t)2;
        else if (i == 3)      cache[i] = (seq_t)6;
        else {
            seq_t N     = (seq_t)i;
            seq_t two   = (seq_t)2, four = (seq_t)4;
            seq_t eleven = (seq_t)11, nine = (seq_t)9, fifteen = (seq_t)15;

            seq_t coeffA = four * ((two * N * N) - (eleven * N) + fifteen);
            seq_t coeffB = four * ((two * N * N) - (nine * N)     + nine);
            seq_t coeffC = (seq_t)2 * (N - (seq_t)3);
            seq_t denom  = N * N - (seq_t)3 * N;

            seq_t numer = coeffA * cache[i-3] + coeffB * cache[i-2] + coeffC * cache[i-1];
            cache[i]    = numer / denom;   /* exact division per recurrence */
        }
    }
    cache_len = n + 1;
    return cache[n];
}
