/*
 * A244777  Prime numbers ending in the prime number 89.
 * https://oeis.org/A244777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime congruent to 89 modulo 100 (i.e., of the
 *            form 100*m+89)
 * Range:     u8 n <= 1, u16 n <= 158, u32 n <= 108, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not defined for n < 1; return 0 as a sentinel */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t m = 0;          /* current m for 100*m+89 */

    while (cache_len < n) {
        /* Grow the cache if necessary */
        if (cache_len + 1 > cache_cap)
            grow_seq(&cache, &cache_cap, cache_len + 1);

        /* Candidate of the form 100*m+89 */
        idx_t cand_idx = 100 * m + 89;
        seq_t candidate = (seq_t)cand_idx;

        if (is_prime_big(candidate))
            cache[cache_len++] = candidate;

        ++m;
    }

    return cache[n - 1];            /* 1‑based indexing */
}
