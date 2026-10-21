/*
 * A292456  Numbers where 6 outnumbers any other digit.
 * https://oeis.org/A292456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute force enumeration of numbers where the decimal digit 6 appears
   more often than any other digit.  The function builds the sequence
   lazily and caches the results so that multiple calls with arbitrary
   indices are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* Static variables for incremental search */
    static seq_t  next_candidate;
    static bool   next_candidate_set = false;

    if (n == 0)   /* undefined, but guard against misuse */
        return 0;

    /* Initialize cache on first use */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Initialize the search candidate when needed */
    if (!next_candidate_set) {
        next_candidate = 6;
        next_candidate_set = true;
    }

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        unsigned char cnt[10] = {0};

        /* Count decimal digits of the current candidate */
        digit_counts(next_candidate, cnt);

        /* Find the maximum count among all digits except 6 */
        unsigned char max_other = 0;
        for (int d = 0; d <= 9; ++d) {
            if (d == 6) continue;
            if (cnt[d] > max_other) max_other = cnt[d];
        }

        /* Store the candidate if 6 occurs more than any other digit */
        if (cnt[6] > max_other) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }

        /* Advance to the next integer candidate */
        next_candidate++;
    }

    return cache[n - 1];
}
