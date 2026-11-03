/*
 * A397858  G.f. A(x) satisfies A(x) = 1 + x * A(x * (1+x)^2).
 * https://oeis.org/A397858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A397858 – a(n) = Sum_{k=0}^{n-1} binomial(2*k, n-k-1) * a(k),  a(0)=1
    The computation uses dynamic programming with memoisation,
    employing the supplied binomial() and ensure_cache() utilities.  */

#include <stdlib.h>
#include "../utils/binomial.h"
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

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;      /* pointer to the cache array */
    static size_t cap   = 0;         /* current capacity of the array   */
    static idx_t  computed = 0;      /* number of terms already computed */

    /* if n has already been computed, return immediately */
    if (n < computed)
        return cache[n];

    /* make sure the cache can hold at least (n+1) elements */
    ensure_cache((idx_t)(n + 1), &cache, &cap);

    /* base case: a(0) = 1 */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
    }

    /* compute all terms from 'computed' up to n */
    for (idx_t m = computed; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t term = binomial(2*k, m - k - 1);
            term *= cache[k];
            sum += term;
        }
        cache[m] = sum;
    }

    computed = n + 1;
    return cache[n];
}
