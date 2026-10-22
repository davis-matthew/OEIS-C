/*
 * A344843  The smallest of three consecutive integers that are products of
 *          three or more primes.
 * https://oeis.org/A344843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer m such that bigomega(m) >= 3 and
 *            bigomega(m+1) >= 3 and bigomega(m+2) >= 3
 * Range:     u8 n <= 10, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_start = 2;  /* start searching from 2 */

    if (n == 0) return 0;  /* undefined for offset 1, but safe */

    /* Grow cache until it contains the requested term */
    while (cache_len < (size_t)n) {
        idx_t m = next_start;
        /* Check that m, m+1, m+2 each have at least 3 prime factors */
        if (bigomega(m)     >= 3 &&
            bigomega(m + 1) >= 3 &&
            bigomega(m + 2) >= 3) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)m;
        }
        ++next_start;
    }
    return cache[n - 1];
}
