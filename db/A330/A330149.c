/*
 * A330149  Expansion of e.g.f. exp(-x) / (1 + log(1 - x)).
 * https://oeis.org/A330149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=1..n} (k-1)! * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
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

seq_t a(idx_t n) {
    /* Cached sequences */
    static seq_t *a_cache   = NULL;
    static size_t a_cap    = 0;
    static size_t a_max    = 0;        /* largest n already computed */

    /* Cached factorials */
    static seq_t *fac       = NULL;
    static size_t fac_cap   = 0;
    static size_t fac_max   = 0;        /* largest k with fac[k] computed */

    /* Make sure there is room for the requested n */
    ensure_cache(n, &a_cache, &a_cap);
    ensure_cache(n, &fac,    &fac_cap);

    /* Initialize base cases if this is the first use */
    if (fac_max == 0) {
        fac[0] = 1;
        fac_max = 0;
    }
    if (a_max == 0) {
        a_cache[0] = 1;
    }

    /* Compute factorials up to n */
    for (idx_t k = fac_max + 1; k <= n; ++k) {
        fac[k] = fac[k - 1] * (seq_t)k;
    }
    fac_max = n;

    /* Compute a(n) for all values from a_max+1 up to n */
    for (idx_t i = a_max + 1; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = 1;                 /* binomial(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* Update binomial coefficient binom = C(i, k) */
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;    /* exact division */

            /* term = (k-1)! * binom * a(i-k) */
            seq_t term = fac[k - 1];
            term = term * binom;
            term = term * a_cache[i - k];

            sum += term;
        }

        /* Add the (-1)^i part */
        if ((i & 1) == 0) {     /* i even -> (+1) */
            sum += (seq_t)1;
        } else {                /* i odd  -> (-1) */
            sum -= (seq_t)1;
        }

        a_cache[i] = sum;
    }
    a_max = n;

    return a_cache[n];
}
