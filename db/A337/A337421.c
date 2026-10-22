/*
 * A337421  Expansion of sqrt((1-6*x+sqrt(1-4*x+36*x^2)) / (2 *
 *          (1-4*x+36*x^2))).
 * https://oeis.org/A337421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-2)^(n-k) * binomial(2*k,k) *
 *            binomial(2*n,2*k) recurrence: a(0)=1, a(1)=0 for n>1:
 *            n*(2*n-1)*(4*n-5)*a(n) = (4*n-3)*(8*n^2-12*n+4)*a(n-1) -
 *            36*(n-1)*(2*n-3)*(4*n-1)*a(n-2)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 25, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifndef __cplusplus
#include <stdint.h>
#endif

static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static size_t  cache_len = 0;

/* Compute a single term using the recurrence, handling integer and
   arbitrary‑precision seq_t types separately. */
static inline seq_t rec_term(idx_t ii, seq_t a1, seq_t a2)
{
#ifdef __cplusplus
    /* Use high‑precision intermediate for integral seq_t types. */
    if constexpr (std::is_integral<seq_t>::value) {
        __int128 t1 = (__int128)(4*ii - 3);
        t1 = t1 * (__int128)(8*ii*ii - 12*ii + 4);
        t1 = t1 * (__int128)a1;

        __int128 t2 = (__int128)36;
        t2 = t2 * (ii-1);
        t2 = t2 * (2*ii - 3);
        t2 = t2 * (4*ii - 1);
        t2 = t2 * (__int128)a2;

        __int128 num = t1 - t2;
        __int128 den = (__int128)ii * (2*ii - 1) * (4*ii - 5);
        return (seq_t)(num / den);
    } else {
        /* Non‑integral seq_t (arbitrary precision).  Multiplication is safe. */
        seq_t t1 = (seq_t)(4*ii - 3) * (seq_t)(8*ii*ii - 12*ii + 4) * a1;
        seq_t t2 = (seq_t)36 * (ii-1) * (2*ii - 3) * (4*ii - 1) * a2;
        seq_t num = t1 - t2;
        seq_t den = (seq_t)ii * (2*ii - 1) * (4*ii - 5);
        return num / den;                /* exact division */
    }
#else
    /* C build – assume unsigned or signed 64‑bit seq_t. */
    __int128 t1 = (__int128)(4*ii - 3);
    t1 = t1 * (__int128)(8*ii*ii - 12*ii + 4);
    t1 = t1 * (__int128)a1;

    __int128 t2 = (__int128)36;
    t2 = t2 * (ii-1);
    t2 = t2 * (2*ii - 3);
    t2 = t2 * (4*ii - 1);
    t2 = t2 * (__int128)a2;

    __int128 num = t1 - t2;
    __int128 den = (__int128)ii * (2*ii - 1) * (4*ii - 5);
    return (seq_t)(num / den);
#endif
}

seq_t a(idx_t n)
{
    /* Allocate/expand cache lazily. */
    if (cache == NULL) {
        size_t init_cap = 2;
        while (init_cap <= (size_t)n) init_cap <<= 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        cache_cap = init_cap;
        cache[0] = (seq_t)1;            /* a(0) */
        cache[1] = (seq_t)0;            /* a(1) */
        cache_len = 2;
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        idx_t ii = (idx_t)i;
        if (ii == 0) continue;
        if (ii == 1) continue;
        cache[ii] = rec_term(ii, cache[ii-1], cache[ii-2]);
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
