/*
 * A394843  G.f. A(x) satisfies A(x) = 1 / (1 - x/2 * d/dx log(1 + 2*x*A(x))).
 * https://oeis.org/A394843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A394843
   Recurrence: a(0)=1, a(1)=1, a(2)=1;
               a(n)=2*a(n-1)+((n-3)/2)*∑_{k=0}^{n-1} a(k)*a(n-1-k)  for n≥3
   Implementation uses memoisation; the recurrence is evaluated only for
   n≥3.  128‑bit arithmetic is employed when compiling for C with
   64‑bit unsigned integers so that intermediate products do not
   overflow.  For builds that use an arbitrary‑precision seq_t type
   the normal arithmetic operators are sufficient.  The function is
   independent of the call order and computes any requested index.
*/

#include <stdlib.h>
#include <stdint.h>

#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
#define USE_INT128 1
typedef unsigned __int128 u128_t;
#endif

static seq_t *cache   = NULL;   /* array of sequence values */
static size_t  cache_cap = 0;   /* allocated capacity in elements */
static idx_t   last_computed = 0; /* next index to compute (count) */

seq_t a(idx_t n)
{
    /* Ensure base cases are initialised for any n<3 */
    if (n < 3) {
        if (last_computed < 3) {
            /* allocate space for indices 0,1,2 */
            if (cache_cap < 3) {
                /* grow to at least 3 elements */
                size_t need = 3;
                while (cache_cap < need) cache_cap = cache_cap ? cache_cap * 2 : 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[0] = cache[1] = cache[2] = (seq_t)1;
            last_computed = 3;
        }
        return (seq_t)1;
    }

    /* initialise base values if this is the first time we reach n≥3 */
    if (last_computed < 3) {
        if (cache_cap < 3) {
            size_t need = 3;
            while (cache_cap < need) cache_cap = cache_cap ? cache_cap * 2 : 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        last_computed = 3;
    }

    /* grow the cached array as needed */
    if (n + 1 > cache_cap) {
        size_t need = n + 1;
        while (cache_cap < need) cache_cap = cache_cap ? cache_cap * 2 : 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute all terms from last_computed up to n */
    for (idx_t i = last_computed; i <= n; ++i) {
#ifdef USE_INT128
        u128_t sum128 = 0;
        idx_t   j = i - 1;
        for (idx_t k = 0; k <= j; ++k) {
            sum128 += (u128_t)cache[k] * (u128_t)cache[j - k];
        }
        u128_t term128 = (u128_t)(i - 3) * sum128 / 2;
        u128_t prev2 = (u128_t)cache[i - 1] * 2;
        cache[i] = (seq_t)(prev2 + term128);
#else
        seq_t sum = 0;
        idx_t j = i - 1;
        for (idx_t k = 0; k <= j; ++k) {
            sum += cache[k] * cache[j - k];
        }
        seq_t term = ((seq_t)(i - 3) * sum) / 2;
        cache[i] = cache[i - 1] * 2 + term;
#endif
    }
    last_computed = n + 1;
    return cache[n];
}
