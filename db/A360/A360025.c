/*
 * A360025  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(n-2*k,k) *
 *          Catalan(k).
 * https://oeis.org/A360025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 26, i32 n <= 48, i64 n <= 89, i128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A360025 – a(n) = 1 – ∑_{k=0}^{n‑3} a(k)·a(n‑k‑3)
   a(0)=a(1)=a(2)=1
   (Offset 0 – a(0) is the first term.) */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h> /* for SIZE_MAX */

/* Persistent cache of computed terms */
static seq_t *cache      = NULL;   /* array of already computed a(i) values */
static size_t cache_cap  = 0;      /* allocated capacity (≥ #elements) */
static size_t cache_len  = 0;      /* number of entries already computed */

/* Ensure space for indices 0…n (i.e. at least n+1 entries). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* space required */
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        /* Grow until enough space */
        while (new_cap < need) {
            if (new_cap > SIZE_MAX / 2) { /* avoid overflow */
                new_cap = need;          /* just enough */
                break;
            }
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term of A360025. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* If already computed, return it. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = 1;                     /* a(0)=a(1)=a(2)=1 */
        } else {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i - 3; ++k)
                sum += cache[k] * cache[i - k - 3];
            cache[i] = 1 - sum;
        }
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
