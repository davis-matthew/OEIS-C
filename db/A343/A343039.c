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
 * Helpers:   ensure_cache, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; for n > 1, a(n) is the smallest positive integer such that
   a(n-1) + n + a(n) is a perfect square. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */
#include "../utils/ensure_cache.h" /* void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr); */

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
