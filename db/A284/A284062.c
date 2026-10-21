/*
 * A284062  Numbers whose smallest decimal digit is 1.
 * https://oeis.org/A284062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   has_min_digit_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer whose smallest decimal digit equals 1 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_min_digit_one.h"

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 1;  /* next number to examine */

    /* Expand cache to contain at least n elements */
    while (cache_len < n) {
        /* Examine next_candidate for property */
        if (has_min_digit_one(next_candidate)) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* OOM protection */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        /* Proceed to next integer */
        next_candidate++;
    }
    return cache[n - 1];
}
