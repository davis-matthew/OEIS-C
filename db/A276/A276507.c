/*
 * A276507  E.g.f.: exp(10*(exp(x)-1)).
 * https://oeis.org/A276507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 * Σ_{k=0}^{n-1} C(n-1,k) * a(k) * Initial condition:
 *            a(0) = 1 * This recurrence follows from the generating‑function
 *            identity * 10*(x/(1-x))*A(x/(1-x)) = A(x)-1. It allows O(n²)
 *            overall * computation with a simple memoised table. The function
 *            is * memoised so that calls in any order are...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 495 (2026-11-03)
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

/* External linkage */
seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *cache_arr = NULL;
    static size_t cache_cap = 0;
    static idx_t  last_computed = 0;   /* highest n that has been computed */

    /* initialise on first call */
    if (cache_arr == NULL) {
        ensure_cache(0, &cache_arr, &cache_cap);  /* guarantee element 0 */
        cache_arr[0] = (seq_t)1;          /* a(0) = 1 */
        last_computed = 0;
    }

    /* extend the table if necessary */
    if (n > last_computed) {
        /* allocate enough entries for index n */
        ensure_cache(n, &cache_arr, &cache_cap);

        /* compute a(k) for k = last_computed+1 .. n */
        for (idx_t curr = last_computed + 1; curr <= n; ++curr) {
            seq_t sum = 0;
            seq_t binom = 1;                /* C(curr-1,0) */
            for (idx_t k = 0; k < curr; ++k) {
                sum += binom * cache_arr[k];

                /* update binomial coefficient to C(curr-1,k+1) */
                if (k + 1 < curr) {
                    /* binom = binom * (curr-1-k) / (k+1) */
                    binom = (binom * (seq_t)(curr - 1 - k)) / (seq_t)(k + 1);
                }
            }
            cache_arr[curr] = sum * (seq_t)10;
        }
        last_computed = n;
    }

    return cache_arr[n];
}
