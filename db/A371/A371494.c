/*
 * A371494  G.f. A(x) satisfies A(x) = 1 / (1 - x*A(x) / (1+x))^2.
 * https://oeis.org/A371494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A371494
   Formula implemented via the linear recurrence
   with initial values a(0)=1, a(1)=2, a(2)=5:
   for n ≥ 0:
   (4*n^2+30*n+56)*a(n+3) =
     (23*n^2+23*n)*a(n) + (42*n^2+151*n+109)*a(n+1) + (15*n^2+98*n+158)*a(n+2).
   The function memoises computed values in a static array which grows
   as needed.  The implementation uses 128‑bit arithmetic for
   built‑in integer builds to avoid overflow in intermediate products.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
    /* In C++ big‑integer builds, seq_t is a class with the usual
       integer operators.  Memory for seq_t[] must be allocated with
       operator new. */
#  include <cstddef>
#endif

/* Static cache for memoised values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;          /* number of elements allocated */
static idx_t next_computed = 0;       /* first index that has not yet been computed */

/* Ensure the cache contains indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = n + 1;
    seq_t *new_cache;

#ifdef __cplusplus
    new_cache = new seq_t[new_size];
    /* Copy previously stored values */
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    delete [] cache;
#else
    new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
    memcpy(new_cache, cache, cache_size * sizeof(seq_t));
    free(cache);
#endif

    cache = new_cache;
    cache_size = new_size;
}

/* 128‑bit arithmetic for built‑in integer builds.  If seq_t is not
    a built‑in type this block is ignored. */
#ifndef __cplusplus
    typedef __uint128_t uint128_t;
#endif

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute any missing values up to n */
    for (idx_t k = next_computed; k <= n; ++k) {
        if (k == 0)          cache[k] = 1;
        else if (k == 1)     cache[k] = 2;
        else if (k == 2)     cache[k] = 5;
        else {
            /* Recurrence: compute a(k) using a(k-3), a(k-2), a(k-1) */
            idx_t m = k - 3;  /* m = k-3 */

            /* Polynomial coefficients (positive values) */
            idx_t c1 = 23*m*m + 23*m;               /* 23*n^2+23*n  */
            idx_t c2 = 42*m*m + 151*m + 109;        /* 42*n^2+151*n+109 */
            idx_t c3 = 15*m*m + 98*m + 158;         /* 15*n^2+98*n+158  */
            idx_t d  = 4*m*m + 30*m + 56;           /* 4*n^2+30*n+56    */

#ifdef __cplusplus
            /* Elementary integer operations on big integers */
            seq_t num = c1 * cache[k-3] + c2 * cache[k-2] + c3 * cache[k-1];
            cache[k]  = num / (seq_t)d;
#else
            /* 128‑bit intermediate to avoid overflow for builtin types */
            uint128_t num = (uint128_t)cache[k-3] * c1
                          + (uint128_t)cache[k-2] * c2
                          + (uint128_t)cache[k-1] * c3;
            cache[k] = (seq_t)(num / d);
#endif
        }
    }

    next_computed = n + 1;
    return cache[n];
}
