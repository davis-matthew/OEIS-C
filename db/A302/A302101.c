/*
 * A302101  G.f. A(x) satisfies: A(x) = 1 + x * (A(x) + 4*x*A'(x)) / (A(x) +
 *          x*A'(x)).
 * https://oeis.org/A302101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1 For n ≥ 2: a(n) = (4n-3)·a(n-1) –
 *            Σ_{k=1}^{n-1} (k+1)·a(k)·a(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 381 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* memoised array of already computed terms */
static seq_t *cache = NULL;     /* array of coefficients */
static size_t cache_cap = 0;    /* allocated size of the array          */
static size_t computed   = 0;   /* number of terms that are already set  */

/* compute a(n) -------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* make sure the array is large enough */
    if (cache_cap <= (size_t)n) {
        ensure_cache(n, &cache, &cache_cap);
    }

    /* initialise a(0) on first use */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    /* if we already have the value, just return it */
    if (n < computed) {
        return cache[n];
    }

    /* a(1) needs to be inserted, but only if n ≥ 1 */
    if (computed == 1 && n >= 1) {
        cache[1] = (seq_t)1;
        computed = 2;
    }

    /* compute successive terms up to n */
    for (idx_t i = (idx_t)computed; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            seq_t term = cache[k];
            term = term * (seq_t)(k + 1);        /* multiply by (k+1)   */
            term = term * cache[i - k];          /* multiply by a_{n-k}  */
            sum += term;                         /* accumulate          */
        }
        seq_t d = ((seq_t)(4 * (seq_t)i - 3)) * cache[i - 1];
        cache[i] = d - sum;
    }

    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
