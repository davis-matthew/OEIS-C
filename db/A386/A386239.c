/*
 * A386239  G.f. A(x) satisfies A(x) = 1/( (1+x) * (1 - x^2*A'(x)) ).
 * https://oeis.org/A386239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 22, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A386239
 *
 *  Formula: a(n) = (-1)^n + Sum_{k=0..n-1} k * a(k) * a(n-1-k)
 *
 *  The implementation uses memoisation: previously computed terms are
 *  stored in a static array that is grown as needed.  The recurrence is
 *  evaluated iteratively for the missing indices.  The code works for
 *  ordinary fixed‑size integer types as well as the arbitrary‑precision
 *  seq_t type (compiled as C++17).  Only integer arithmetic is used.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;    /* pointer to array of computed terms */
static idx_t cache_len = 0;    /* number of elements stored (highest index + 1) */

/* enlarge the cache to hold at least new_len elements,
 * copying any previously stored values.  The array is
 * allocated with the appropriate form for C or C++.
 */
static void resize_cache(idx_t new_len) {
    if (cache_len >= new_len) return;   /* already sufficient */

    seq_t *new_cache = NULL;

#ifdef __cplusplus
    new_cache = new seq_t[new_len];     /* value‑initialise for C++ */
#else
    new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
#endif

    /* copy old values */
    for (idx_t i = 0; i < cache_len; ++i) {
        new_cache[i] = cache[i];
    }

    /* free old storage */
    if (cache != NULL) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache     = new_cache;
    cache_len = new_len;
}

/* fill cache[cache_len .. n] with successive terms of the sequence  */
static void compute_to(idx_t n) {
    if (cache_len > n) return;        /* requested index already computed */

    idx_t old_len = cache_len;        /* start of new region after reallocation */
    resize_cache(n + 1);              /* ensure space for n (inclusive) */

    for (idx_t i = old_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;      /* a(0) = 1  */
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t prod = cache[k] * cache[i - 1 - k];
                prod *= (seq_t)k;
                sum += prod;
            }
            seq_t sign = ((i & 1) == 0) ? (seq_t)1 : (seq_t)-1;
            cache[i]   = sign + sum;
        }
    }
}

seq_t a(idx_t n) {
    compute_to(n);
    return cache[n];
}
