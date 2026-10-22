/*
 * A346740  Expansion of e.g.f.: exp(exp(x) - 5*x - 1).
 * https://oeis.org/A346740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = -5 * a(n-1) + Sum_{k=0..n-1} binomial(n-1, k) *
 *            a(k) for n ≥ 1
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

/* static cache of previously computed terms – external linkage is
   only via the function a(idx_t) below. */
static seq_t *cache = NULL;   /* pointer to array of seq_t */
static idx_t  cache_len = 0;  /* number of computed entries (next free index) */
static idx_t  cache_cap = 0;  /* physical capacity of the array */

/* Ensure that cache[n] (and cache[0..n]) are computed and stored. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;                     /* already available */

    /* enlarge the array if necessary */
    idx_t new_cap = n + 1;
    seq_t *new_cache;

    if (cache == NULL) {
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = calloc(new_cap, sizeof(seq_t));
#endif
    } else {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        delete[] cache;
        new_cache = tmp;
#else
        new_cache = realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL)
            exit(1);
#endif
    }

    cache      = new_cache;
    cache_cap  = new_cap;

    /* compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {          /* base case */
            cache[0] = 1;
            continue;
        }

        /* recurrence:
           a(i) = -5 * a(i-1) + Σ_{k=0..i-1} C(i-1,k) * a(k) */
        seq_t sum = ((seq_t)-5) * cache[i-1];
        seq_t binom = 1;          /* C(i-1,0) */

        for (idx_t k = 0; k < i; ++k) {
            sum += binom * cache[k];
            if (k + 1 < i)
                binom = binom * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
        }

        cache[i] = sum;
    }

    cache_len = n + 1;
}

/* External function requested by the build system. */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
