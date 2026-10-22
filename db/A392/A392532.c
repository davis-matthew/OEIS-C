/*
 * A392532  G.f. A(x) satisfies: A(x) = 1 + x * A(2*x/(1 - x)^2) / (1 - x)^2.
 * https://oeis.org/A392532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n+k, n-k-1) * 2^k * a(k) (a(0) =
 *            1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_computed = 0;

    /* Ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first term if not already done */
    if (cache[0] == 0) {
        cache[0] = 1;
        last_computed = 0;
    }

    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            seq_t pow2 = 1;
            for (idx_t k = 0; k < i; ++k) {
                seq_t b = binomial(i + k, i - k - 1);
                seq_t term = b * pow2 * cache[k];
                sum += term;
                pow2 *= 2;
            }
            cache[i] = sum;
            last_computed = i;
        }
    }

    return cache[n];
}
