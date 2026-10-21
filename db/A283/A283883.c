/*
 * A283883  Relative of Hofstadter Q-sequence: a(n) = max(0, n+117) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for n > 0.
 * https://oeis.org/A283883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = max(0, n+117) for n <= 0; a(n) = a(n-a(n-1)) +
 *            a(n-a(n-2)) for n > 0
 * Range:     u8 n <= 34, u16 n <= 192, u32 n <= 9770, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base_value_int64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/base_value_int64.h"

/* Cache for computed values.  cache[0] is unused; indices start at 1. */
static seq_t *cache = NULL;
static idx_t cached_max = 0;

/* Ensure that cache[] holds entries up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n <= cached_max)
        return;
    idx_t old_max  = cached_max;
    idx_t new_max  = n;
    seq_t *new_cache;

#ifdef __cplusplus
    new_cache = new seq_t[new_max + 1];
#else
    new_cache = malloc((size_t)(new_max + 1) * sizeof(seq_t));
#endif

    /* Initialise everything to zero. */
    for (idx_t i = 0; i <= new_max; ++i)
        new_cache[i] = (seq_t)0;

    /* Copy already computed values. */
    if (cache) {
        for (idx_t i = 0; i <= old_max; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
    }

    cache       = new_cache;
    cached_max  = new_max;
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* safety for accidental callers */

    /* If already computed, return immediately. */
    if (n <= cached_max)
        return cache[n];

    idx_t start = cached_max + 1;   /* first index that needs to be computed. */
    ensure_cache(n);

    for (idx_t i = start; i <= n; ++i) {
        seq_t a_im1 = (i >= 2) ? cache[i-1]
                               : base_value_int64((int64_t)(i-1));
        seq_t a_im2 = (i >= 3) ? cache[i-2]
                               : base_value_int64((int64_t)(i-2));

        int64_t idx1 = (int64_t)i - (int64_t)a_im1;
        int64_t idx2 = (int64_t)i - (int64_t)a_im2;

        seq_t v1 = (idx1 > 0) ? cache[(idx_t)idx1]
                              : base_value_int64(idx1);
        seq_t v2 = (idx2 > 0) ? cache[(idx_t)idx2]
                              : base_value_int64(idx2);

        cache[i] = v1 + v2;
    }
    return cache[n];
}
