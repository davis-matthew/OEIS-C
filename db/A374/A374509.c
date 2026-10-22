/*
 * A374509  Expansion of 1/(1 - 2*x + 5*x^2)^(7/2).
 * https://oeis.org/A374509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 17, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A374509: Expansion of 1/(1 - 2*x + 5*x^2)^(7/2)
   Recurrence: a(0)=1, a(1)=7,
                a(n) = ((2*n+5)*a(n-1) - 5*(n+5)*a(n-2)) / n     (n >= 2)
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"

static seq_t *cache = NULL;      /* dynamic array of computed terms           */
static size_t cache_len = 0;     /* number of terms currently stored          */
static size_t cache_cap = 0;     /* allocated capacity of the array           */

/* ensure at least `needed` elements can be stored */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;                     /* double until enough                     */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);               /* out of memory                            */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise with the first two terms when required */
    if (cache_len == 0) {
        ensure_capacity(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_len = 2;
    }

    /* extend the cache up to the requested index */
    if ((size_t)n >= cache_len) {
        size_t need = (size_t)n + 1;
        ensure_capacity(need);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term1 = (seq_t)(2 * (seq_t)i + (seq_t)5);   /* 2*i+5          */
            seq_t term2 = (seq_t)(5 * (seq_t)(i + 5));       /* 5*(i+5)        */
            seq_t prod1 = seq_mul(term1, cache[i - 1]);      /* (2i+5)*a(i-1)  */
            seq_t prod2 = seq_mul(term2, cache[i - 2]);      /* 5(i+5)*a(i-2)  */
            seq_t num   = prod1 - prod2;                    /* numerator      */
            cache[i]    = num / (seq_t)i;                   /* division by n  */
        }
        cache_len = need;     /* now we have stored terms 0 .. n           */
    }

    return cache[(size_t)n];
}
