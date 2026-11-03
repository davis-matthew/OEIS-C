/*
 * A343039  a(1) = 1, for n > 1, a(n) is the smallest positive integer for
 *          which a(n-1) + n + a(n) is a square.
 * https://oeis.org/A343039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; for n > 1, a(n) is the smallest positive integer such that
   a(n-1) + n + a(n) is a perfect square. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
    /* n is guaranteed to be >= 1 (offset 1) */
    static seq_t *cache = NULL;   /* 0‑based array storing a(1), a(2), … */
    static size_t cache_cap = 0;  /* allocated size of cache */
    static idx_t cache_size = 0;  /* number of terms currently stored */

    /* Guarantee space for n terms.  ensure_cache enlarges the array
       if necessary and leaves existing values untouched. */
    ensure_cache(n, &cache, &cache_cap);

    /* If a(1) has never been computed, initialise it. */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
        /* If the request is only for a(1), we are done. */
        if (n == 1)
            return cache[0];
    }

    /* If the requested term is already computed, return it. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Compute missing terms incrementally. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* previous term is a(i-1) = cache[i-2] */
        seq_t prev = cache[i - 2];
        /* sum = a(i-1) + i */
        seq_t sum = prev + (seq_t)i;

        /* smallest integer k such that k^2 > sum */
        seq_t k = floor_sqrt(sum) + 1;

        /* a(i) = k^2 - sum  (positive by construction) */
        cache[i - 1] = k * k - sum;
    }

    cache_size = n;
    return cache[n - 1];
}
