/*
 * A276530  a(n) = (a(n-1) * a(n-5) + a(n-3)^3) / a(n-6), a(0) = a(1) = ... =
 *          a(5) = 1.
 * https://oeis.org/A276530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1) * a(n-5) + a(n-3)^3) / a(n-6) with a(0) = a(1) =
 *            a(2) = a(3) = a(4) = a(5) = 1
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 15, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   free_cache, seq_alloc_new_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_cache.h"

/* Return the nth term of sequence A276530. */
seq_t a(idx_t n)
{
    /* Static cache: values already computed. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* number of elements allocated */
    static idx_t last_computed = 5; /* last index for which we know the value */

    /* The first six terms are 1. */
    if (n <= 5) {
        return (seq_t)1;
    }

    /* Grow the cache if needed. */
    if (cache == NULL) {
        cache_cap = (n >= 5) ? (n + 1) : 6;
        cache = seq_alloc_new_array(cache_cap);
        for (idx_t i = 0; i < 6; ++i) {
            cache[(size_t)i] = (seq_t)1;
        }
        last_computed = 5;
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap = new_cap * 2 + 1;
        }
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = cache[i];
        }
        free_cache(cache);
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute new terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Indices of the required previous terms. */
        idx_t i1 = i - 1;
        idx_t i5 = i - 5;
        idx_t i3 = i - 3;
        idx_t i6 = i - 6;

        /* a(i-1) * a(i-5) */
        seq_t prod15 = seq_mul(cache[(size_t)i1], cache[(size_t)i5]);

        /* (a(i-3))^3 */
        seq_t cube = cache[(size_t)i3];
        cube = seq_mul(cube, cube);
        cube = seq_mul(cube, cache[(size_t)i3]);

        /* Numerator: prod15 + cube */
        seq_t numerator = prod15 + cube;

        /* Denominator: a(i-6) */
        seq_t denom = cache[(size_t)i6];

        /* The division is exact, so truncating division is fine. */
        cache[(size_t)i] = numerator / denom;
    }

    last_computed = n;
    return cache[(size_t)n];
}
