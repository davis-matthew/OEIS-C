/*
 * A297148  Numbers whose base-10 digits d(m), d(m-1),..., d(0) have m=0 or
 *          else d(i) = d(i+1) for some i in {0,1,...,m-1}.
 * https://oeis.org/A297148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   has_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer whose decimal representation has
   at least one pair of consecutive equal digits; computed by brute‑force
   search. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_consecutive_equal_digits.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_len = 0;
    static uint64_t next_candidate = 1; /* next integer to test */

    /* grow cache when necessary (doubling strategy) */
    if (cache_len < (size_t)n) {
        if (!cache) {
            cache_capacity = 1024;
            cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        } else if (cache_len + 1 > cache_capacity) {
            size_t new_cap = cache_capacity * 2;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_capacity = new_cap;
        }

        /* generate terms until we have at least n of them */
        while (cache_len < (size_t)n) {
            if (has_consecutive_equal_digits(next_candidate)) {
                cache[cache_len++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
