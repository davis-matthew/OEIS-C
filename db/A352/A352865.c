/*
 * A352865  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} (-1)^k *
 *          binomial(n-k,k) * a(n-2*k-1).
 * https://oeis.org/A352865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (-1)^k * binomial(n-k, k) *
 *            a(n-2*k-1); a(0) = 1
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 23, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
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
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = (idx_t)-1;

seq_t a(idx_t n)
{
    /* Make sure we have space for indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialize first term if this is the first call. */
    if (last_computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t maxk = (i > 0) ? (i - 1) / 2 : 0;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t bin = binomial(i - k, k);
            seq_t term = bin * cache[i - 2 * k - 1];
            if (k & 1)
                sum -= term;
            else
                sum += term;
        }
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
