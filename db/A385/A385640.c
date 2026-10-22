/*
 * A385640  Numbers k such that the sum of the digits of k divides k and the
 *          sum of the digits of k^2 divides k^2.
 * https://oeis.org/A385640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k such that digit_sum(k) divides k and digit_sum(k²) divides k². */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/digit_sum.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of stored terms   */
    static size_t cache_cap  = 0;   /* allocated capacity      */

    /* Next integer to test (1‑based) */
    static seq_t current = (seq_t)1;

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t k   = current;
        seq_t s   = digit_sum(k);

        if (s != (seq_t)0 && k % s == (seq_t)0) {
            seq_t sq = k * k;
            seq_t s2 = digit_sum(sq);

            if (s2 != (seq_t)0 && sq % s2 == (seq_t)0) {
                append_candidate(&cache, &cache_len, &cache_cap, k);
            }
        }

        current = current + (seq_t)1;
    }

    return cache[n - 1];
}
