/*
 * A256086  Non-palindromic balanced numbers in base 6.
 * https://oeis.org/A256086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_balanced_nonpal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by exhaustive search of balanced base-6 non‑palindromic numbers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_balanced_nonpal.h"

seq_t a(idx_t n)
{
    static seq_t next_candidate = 1;
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    while (cache_size < n) {
        if (is_balanced_nonpal(next_candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
