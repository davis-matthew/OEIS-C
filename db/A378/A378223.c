/*
 * A378223  Inverse Möbius transform of A345182.
 * https://oeis.org/A378223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + Σ_{d|n, 3≤d<n} a(d) for n ≥ 3, a(1)=a(2)=1
 * Range:     u8 n <= 143, u16 n <= 8639, u32 n <= 9899, u64 n <= 9899,
 *            u128 n <= 9899, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *cache = NULL;     /* array of computed values */
    static size_t cache_cap = 0;    /* allocated capacity, guaranteed ≥ n+1 */
    static size_t computed = 0;     /* largest index that has been filled */

    /* Make sure the cache is big enough for index 'n' */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute any missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            cache[i] = 1;
        } else {
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    idx_t d1 = d;
                    idx_t d2 = i / d;
                    if (d1 >= 3 && d1 < i) sum += cache[d1];
                    if (d2 != d1 && d2 >= 3 && d2 < i) sum += cache[d2];
                }
            }
            cache[i] = sum + 2;
        }
    }
    computed = n;
    return cache[n];
}
