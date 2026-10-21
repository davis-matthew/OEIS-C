/*
 * A242012  a(n) is the number of positive integers k <= n for which gpf(k^2 +
 *          1) = gpf(n^2 + 1), where gpf is the greatest prime divisor.
 * https://oeis.org/A242012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute force: a(n) = |{ k | 1 ≤ k ≤ n , gpf(k²+1) = gpf(n²+1) }|     */
/* Uses the gpf(uint64_t) utility.                                             */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 for the sequence; n == 0 is irrelevant but handled safely. */
    if (n == 0) return 0;

    /* Static cache of gpf(k²+1) values up to the largest n seen.          */
    static uint64_t *gpf_cache = NULL;
    static size_t     gpf_cap   = 0;

    if ((idx_t)n >= gpf_cap) {
        size_t new_cap = (size_t)n + 1;
        uint64_t *tmp  = (uint64_t*)realloc(gpf_cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                         /* allocation failed   */
        gpf_cache = tmp;
        for (size_t i = gpf_cap; i < new_cap; ++i) {
            uint64_t sq = (uint64_t)i * (uint64_t)i + 1;
            gpf_cache[i] = gpf(sq);
        }
        gpf_cap = new_cap;
    }

    /* Count k ≤ n with the same greatest prime factor.                     */
    seq_t count = 0;
    uint64_t gpf_n = gpf_cache[(size_t)n];
    for (idx_t k = 1; k <= n; ++k) {
        if (gpf_cache[(size_t)k] == gpf_n)
            ++count;
    }
    return count;
}
