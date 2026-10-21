/*
 * A289780  p-INVERT of the positive integers (A000027), where p(S) = 1 - S -
 *          S^2.
 * https://oeis.org/A289780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289780:  p-INVERT of the positive integers with p(S)=1‑S‑S²
   a(n) = 5*a(n‑1) – 7*a(n‑2) + 5*a(n‑3) – a(n‑4)
   Initial values:
       a(0)=1
       a(1)=4
       a(2)=14
       a(3)=47
   Computation is memoised; calls may be in any order. */

#include <stdint.h>
#include <stdlib.h>

/* Helper to replace C++ new/delete in C. */
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;       /* array of computed terms      */
static idx_t  cache_size = 0;     /* number of terms stored       */
static size_t cache_cap  = 0;     /* allocated capacity (elements) */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap  = 4;
        cache_size = 4;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)47;
    }

    /* if the requested index is already known, return it */
    if (n < cache_size)
        return cache[n];

    /* grow array if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) {
            new_cap *= 2;
        }
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
#endif
        cache_cap = new_cap;
    }

    /* compute new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = (seq_t)5 * cache[i-1];
        term += (seq_t)5 * cache[i-3];
        term -= (seq_t)7 * cache[i-2];
        term -= cache[i-4];
        cache[i] = term;
    }
    cache_size = n + 1;
    return cache[n];
}
