/*
 * A350792  Number of digraphs on n labeled nodes with a global source (or
 *          sink).
 * https://oeis.org/A350792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*2^((n-1)^2) - Σ_{k=1}^{n-1}
 *            binomial(n,k)*2^((n-2)*(n-k))*a(k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   binomial, ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ensure_cache.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute a(n) for n >= 1.  Uses memoisation so that calls in any order are fast. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Make sure the cache can hold all values up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise base case.  a(1) = 1.  Zero means “not yet computed”. */
    if (cache[1] == 0)
        cache[1] = 1;

    /* Fill in any missing values up to n. */
    for (idx_t m = 2; m <= n; ++m)
    {
        if (cache[m] != 0) continue;                 /* already computed */

        seq_t term = (seq_t)m * pow2((m - 1) * (m - 1));

        seq_t sum = 0;
        for (idx_t k = 1; k < m; ++k)
        {
            seq_t prod = binomial(m, k);
            prod *= pow2((m - 2) * (m - k));
            prod *= cache[k];
            sum += prod;
        }

        cache[m] = term - sum;
    }

    return cache[n];
}
