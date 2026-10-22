/*
 * A352435  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n,2*k+1) *
 *          a(k) * a(n-2*k-1).
 * https://oeis.org/A352435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n, 2*k+1) *
 *            a(k) * a(n-2*k-1) (OEIS A352435)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* Handle the base case directly */
    if (n == 0) {
        return 1;
    }

    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure the cache is large enough */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base case in the cache */
    cache[0] = 1;

    /* Compute values up to n iteratively, skipping already computed entries */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) {
            continue; /* already computed */
        }

        idx_t limit = (i - 1) / 2;
        seq_t sum = 0;

        /* Current binomial coefficient binomial(i, 2*k+1),
           starting with k = 0 => binomial(i,1) = i */
        seq_t binom = (seq_t)i;

        for (idx_t k = 0; k <= limit; ++k) {
            /* term = binom * a(k) * a(i - 2*k - 1) */
            seq_t term = binom * cache[k];
            term = term * cache[i - 2 * k - 1];
            sum += term;

            /* Update binom for the next odd index, unless this is the last term */
            if (k < limit) {
                idx_t j = 2 * k + 1;
                idx_t next_j = j + 2;

                seq_t num = binom * (seq_t)(i - j);
                num = num * (seq_t)(i - j - 1);
                seq_t den = (seq_t)next_j * (seq_t)(next_j - 1);

                binom = num / den; /* exact division */
            }
        }

        cache[i] = sum;
    }

    return cache[n];
}
