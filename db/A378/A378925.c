/*
 * A378925  Positive numbers whose squares have at most one digit less than 8.
 * https://oeis.org/A378925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = the n-th positive integer whose square has at most one
 *            digit less than 8 (in decimal). Computed by scanning candidates
 *            sequentially and testing the property
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* Cache of previously found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* Next candidate to test (persist between calls) */
    static seq_t candidate = 1;

    if (n == 0) return 0;  /* offset is 1; index < 1 is undefined */

    /* Ensure enough terms are generated */
    while (cache_len < (size_t)n) {
        /* Grow capacity if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute candidate² and count digits < 8 */
        seq_t sq = candidate * candidate;
        unsigned int freq[10] = {0};
        digit_frequencies(sq, freq);
        unsigned int less = 0;
        for (int d = 0; d <= 7; ++d) less += freq[d];

        /* Accept if at most one such digit */
        if (less <= 1) {
            cache[cache_len++] = candidate;
        }

        ++candidate;
    }

    return cache[n - 1];
}
