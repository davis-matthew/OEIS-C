/*
 * A242454  Triangular numbers T such that sum of digits of T is semiprime.
 * https://oeis.org/A242454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the n-th triangular number T(k) for which the digital sum
 *            of T(k) is a semiprime (product of two primes)
 * Range:     u8 n <= 15, u16 n <= 146, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_semiprime_uint64.h"

/* Return the n-th term of A242454 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; behaviour below irrelevant */

    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_count = 0;
    static size_t cache_capacity = 0;
    static idx_t last_tri_index = 0;   /* last n processed for triangular numbers */

    /* Grow the cache until it contains at least n items */
    if (cache_count < n) {
        if (cache_capacity == 0) {
            cache_capacity = 16;
            cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        }
        while (cache_count < n) {
            /* Next triangular number */
            idx_t i = last_tri_index + 1;
            seq_t tri = (seq_t)i * ((seq_t)i + 1) / 2;
            last_tri_index = i;

            /* Digit sum of the triangular number */
            seq_t ds = digit_sum(tri);
            uint64_t sum = (uint64_t)ds;    /* sum fits in 64 bits */

            /* Keep if the digit sum is a semiprime */
            if (is_semiprime_uint64(sum)) {
                if (cache_count == cache_capacity) {
                    cache_capacity <<= 1;
                    cache = (seq_t *)realloc(cache,
                                              cache_capacity * sizeof(seq_t));
                }
                cache[cache_count++] = tri;
            }
        }
    }
    return cache[n - 1];
}
