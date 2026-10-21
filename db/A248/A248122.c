/*
 * A248122  Number of strings of length n over a three-letter alphabet that
 *          begin with a nontrivial palindrome.
 * https://oeis.org/A248122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(0) = 0, a(1) = 0, a(n) = 3*a(n-1) + 3^ceil(n/2) - a(ceil(n/2))
 *            for n >= 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper for 3^k */
#include "../utils/pow3.h"
#include <stdlib.h>
#include <stddef.h>

/* Computes a(n) for the sequence A248122.
   Uses a static cache so that multiple calls are efficient,
   regardless of the order of calls. */
seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure we have space for a[0 .. n] */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;

        /* Compute any missing entries up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else if (i == 1) {
                cache[1] = 0;
            } else {
                /* ceil(i/2) == (i + 1)/2 */
                idx_t half = (i + 1) / 2;
                cache[i] = (seq_t)3 * cache[i - 1] + pow3(half) - cache[half];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
