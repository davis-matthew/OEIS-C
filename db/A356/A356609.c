/*
 * A356609  Numbers k that can be written as the sum of 6 divisors of k (not
 *          necessarily distinct).
 * https://oeis.org/A356609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k (k ≥ 1) such that k is
 *            divisible by at least one of 6, 8, 10, 14, 44, or 52
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache to store discovered terms */
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_cap = 0;     /* current allocated capacity */
    static size_t cache_len = 0;     /* number of terms stored */
    static idx_t next_candidate = 5; /* last candidate tested (will become 6 on first loop) */

    /* Grow the cache until we have at least n terms */
    while (cache_len < n) {
        /* Ensure there is room for at least one more term */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
            if (!cache) return 0; /* out of memory */
        } else if (cache_len == cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!tmp) return 0;
            cache = tmp;
        }

        /* Find next integer that satisfies the divisibility condition */
        next_candidate++;
        idx_t c = next_candidate;
        if ((c % 6 == 0) || (c % 8 == 0) || (c % 10 == 0) ||
            (c % 14 == 0) || (c % 44 == 0) || (c % 52 == 0)) {
            cache[cache_len++] = (seq_t)c;
        }
    }

    return cache[n - 1];
}
