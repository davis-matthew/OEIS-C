/*
 * A329928  a(n) = (Pi/2)*(2*n+1)!*binomial(2*n+1, (2*n+1)/2).
 * https://oeis.org/A329928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*(4*n)^2; a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   ensure_cache, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  hi    = 0;

    if (cache == NULL) {
        ensure_cache((idx_t)0, &cache, &cap);
        cache[0] = (seq_t)2;
        hi = 0;
    }

    if (n > hi) {
        ensure_cache(n, &cache, &cap);
        for (idx_t i = hi + 1; i <= n; ++i) {
            /* multiplier = (4*i)^2 = 16*i*i */
            seq_t mult = (seq_t)(16ULL * (unsigned long long)i * (unsigned long long)i);
            cache[i] = seq_mul(cache[i - 1], mult);
        }
        hi = n;
    }

    return cache[n];
}
