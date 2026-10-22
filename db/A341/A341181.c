/*
 * A341181  Numbers that when divided by the sum of their digits leave 14 as
 *          remainder.
 * https://oeis.org/A341181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1649, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th positive integer N such that N % sum_of_digits(N) == 14.
   The function searches sequentially for the required term and memoises
   results for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    /* Static cache for terms that have already been found. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    /* The smallest integer that the next search should start from. */
    static seq_t next_start = 1;

    /* Allocate cache on first call. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Keep searching until we have at least n terms cached. */
    while (cache_size < n) {
        seq_t cur = next_start;
        for (;;) {
            /* Compute the digit sum of cur. */
            seq_t sum = digit_sum(cur);
            /* Check the remainder condition. */
            if ((cur % sum) == (seq_t)14) {
                /* Ensure capacity for the new term. */
                if (cache_size == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = cur;
                /* Next search should start after the term we just found. */
                next_start = cur + (seq_t)1;
                break;
            }
            /* Move to the next candidate. */
            cur = cur + (seq_t)1;
        }
    }

    return cache[(size_t)(n - (idx_t)1)];
}
