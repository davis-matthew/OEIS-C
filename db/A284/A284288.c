/*
 * A284288  Numbers n such that the average of the strong divisors of n is an
 *          integer.
 * https://oeis.org/A284288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   strong_avg_integer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A284288: find the n‑th integer whose strong‑divisor average is an integer.
   Implements an on‑demand search with trial division factorisation. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/strong_avg_integer.h"

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_tested = 1;

    /* Grow cache to hold at least n elements. */
    while (cache_len < n) {
        /* Find next integer with integer strong‑divisor average. */
        do {
            ++last_tested;
        } while (!strong_avg_integer(last_tested));

        /* Ensure capacity. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();        /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)last_tested;
    }

    return cache[n - 1];
}
