/*
 * A354254  a(n) is the least m >= 0 such that n = f^k(m) for some k >= 0
 *          (where f^k denotes the k-th iterate of A092391).
 * https://oeis.org/A354254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least m ≥ 0 such that n = f^k(m) for some k ≥ 0,
 * where f(x) = x + popcount(x).  The computation uses dynamic programming
 * over predecessors m = n − k with popcount(m) = k.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;       /* cache[a] holds a(a) for all computed a */
static size_t cache_len = 0;      /* number of values currently computed (highest index+1) */
static size_t cache_cap = 0;      /* allocated capacity of cache[] */

seq_t a(idx_t n)
{
    if ((size_t)n >= cache_len) {
        /* enlarge the cache if necessary */
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            grow_seq_array(&cache, &cache_cap, new_cap);
        }
        /* compute values from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t best = (seq_t)i;          /* m = i itself is always a candidate */
            unsigned int bitlen = 0;
            if (i > 0) {
                bitlen = 64 - __builtin_clzll((unsigned long long)i);
            }
            for (unsigned int k = 1; k <= bitlen; ++k) {
                idx_t m = i - k;
                /* m ≥ 0 guaranteed because k ≤ bitlen ≤ i for i > 0 */
                if (__builtin_popcountll((unsigned long long)m) == k) {
                    seq_t cand = cache[(size_t)m];
                    if (cand < best) best = cand;
                }
            }
            cache[(size_t)i] = best;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
