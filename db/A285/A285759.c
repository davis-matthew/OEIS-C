/*
 * A285759  a(n) = a(n - 1 - a(n - 1)) + a(n - 1 - a(n - 4)), with a(1) = 1,
 *          a(2) = 1, a(3) = 1, a(4) = 2.
 * https://oeis.org/A285759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - 1 - a(n - 1)) + a(n - 1 - a(n - 4)), a(1) = 1, a(2)
 *            = 1, a(3) = 1, a(4) = 2
 * Range:     u8 n <= 508, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9390 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Convert a seq_t value to idx_t.  Works both when seq_t is an
   integer type (C build) and when seq_t is an arbitrary‑precision
   type with a conversion to idx_t (C++ build). */
static inline idx_t seqt_to_idx(seq_t x)
{
    return (idx_t)x;
}

/* 1‑based cache array of computed terms.  cache_cap is the number
   of allocated elements (so the maximum usable index is cache_cap-1). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that the cache contains values up to and including index n. */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n;          /* since n ≥ 1, this fits in size_t */
    if (target + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = target + 1;    /* 1‑based indexing requires +1   */
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;

        /* Initialize base values when the cache is expanded for the
           first time or when the base area has not yet been filled. */
        if (old_cap < 5) {
            if (new_cap > 1) cache[1] = 1;
            if (new_cap > 2) cache[2] = 1;
            if (new_cap > 3) cache[3] = 1;
            if (new_cap > 4) cache[4] = 2;
        }

        /* Compute any new terms that are required. */
        size_t start = (old_cap < 5 ? 5 : old_cap);
        for (size_t k = start; k <= target; ++k) {
            idx_t idx1 = ((idx_t)k - 1) - seqt_to_idx(cache[k - 1]);
            idx_t idx2 = ((idx_t)k - 1) - seqt_to_idx(cache[k - 4]);
            cache[k] = cache[idx1] + cache[idx2];
        }

        cache_cap = new_cap;
    }
}

/* Public interface: return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, but return 0 safely   */
    ensure_cache(n);
    return cache[(size_t)n];
}
