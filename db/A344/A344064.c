/*
 * A344064  Expansion of Product_{k>=1} (1 + 5^(k-1)*x^k).
 * https://oeis.org/A344064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + 5^(k-1)*x^k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed coefficients up to the largest n seen so far. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* If we already have enough pre‑computed values, just return the entry. */
    if (cache && n + 1 <= cache_size) {
        return cache[n];
    }

    /* Resize cache to hold up to n (inclusive). */
    if (cache) free(cache);
    cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!cache) return (seq_t)0;               /* allocation failure */

    cache[0] = (seq_t)1;                      /* a(0) = 1 */
    seq_t pow5 = (seq_t)1;                    /* 5^(k-1) for current k */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* Update coefficients for subset that includes part k. */
        for (idx_t j = n; j >= k; --j) {
            cache[j] += cache[j - k] * pow5;
        }
        pow5 *= (seq_t)5;                     /* prepare 5^k for next iteration */
    }

    cache_size = n + 1;
    return cache[n];
}
