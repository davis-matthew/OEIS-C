/*
 * A342675  a(n) = Sum_{d|n} d^(n-d+1).
 * https://oeis.org/A342675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-d+1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* <big_pow> is provided in the build utilities */
#include "../utils/big_pow.h"
/* <ensure_cache> helper to grow a static cache array */
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    if (n == 0) return 0;            /* offset is 1, caller must use n>=1 */

    ensure_cache(n, &cache, &cache_cap);
    if (cache[n - 1] != 0) return cache[n - 1];

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* term for divisor d */
            seq_t t = big_pow((seq_t)d, (seq_t)(n - d + 1));
            sum += t;
            idx_t e = n / d;
            if (e != d) {
                seq_t t2 = big_pow((seq_t)e, (seq_t)(n - e + 1));
                sum += t2;
            }
        }
    }
    cache[n - 1] = sum;
    return sum;
}
