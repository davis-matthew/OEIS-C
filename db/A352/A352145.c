/*
 * A352145  Expansion of e.g.f. exp(-1 + cos(x) + sin(x)).
 * https://oeis.org/A352145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} (-1)^floor(k/2) *
 *            binomial(n-1,k-1) * a(n-k)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 16, i64 n <= 26, i128 n <= 44,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 542 (2026-11-03)
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_comp = 0;

seq_t a(idx_t n)
{
    if (!cache) {
        /* allocate space for at least the first element */
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = 1;
        last_comp = 0;
    }

    if ((idx_t)n > last_comp) {
        /* make sure we have room for indices up to n */
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t m = last_comp + 1; m <= n; ++m) {
            seq_t sum = 0;
            seq_t binom = 1;   /* binomial(m-1,0) */

            for (idx_t k = 1; k <= m; ++k) {
                seq_t term = binom * cache[m - k];

                /* sign = (-1)^floor(k/2) */
                if (((k >> 1) & 1) == 0)
                    sum += term;
                else
                    sum -= term;

                if (k < m) {
                    binom = (binom * (seq_t)(m - k)) / (seq_t)k;
                }
            }
            cache[m] = sum;
        }
        last_comp = n;
    }

    return cache[n];
}
