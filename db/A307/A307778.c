/*
 * A307778  a(1) = 1; a(n+1) = Sum_{d|n} (-1)^(d+1)*a(d).
 * https://oeis.org/A307778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; * a(n+1) = Sum_{d|n} (-1)^(d+1) * a(d) (n ≥ 1)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory management and types */
#include <stdlib.h>
#include <stddef.h>

/* Utility for reallocating an array of seq_t objects */
#include "../utils/seq_realloc.h"

/* Cache of already computed terms.
 *  cache[0] is unused; valid indices are 1 .. cache_cap-1.
 */
static seq_t *cache      = NULL;   /* array of seq_t */
static size_t cache_cap  = 0;      /* allocated capacity (≥ index + 1) */
static size_t cache_len  = 0;      /* highest index that has a value */

/* Ensure the cache can hold at least “needed” indices (0‑based size). */
static void
ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;                                 /* nothing to do */

    size_t newcap = cache_cap ? cache_cap : 1;
    while (newcap < needed)
        newcap <<= 1;                            /* double until big enough */

    seq_t *tmp = seq_realloc(cache, cache_cap, newcap);
    if (!tmp)
        abort();                                 /* allocation failure */
    cache      = tmp;
    cache_cap  = newcap;
}

/* Return the n-th term of the sequence; n ≥ 1.
 *  The function works for any order of calls and memoises results.
 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                                 /* undefined for n==0 */

    /* Initialise the cache for the first term if necessary. */
    if (cache_len == 0) {
        ensure_cache_cap(2);                      /* at least space for 1 */
        cache[1] = 1;
        cache_len = 1;
    }

    /* Make room for the requested index.  We need at least index n. */
    if ((size_t)n >= cache_cap)
        ensure_cache_cap((size_t)n + 1);

    /* Compute any missing values up to n. */
    if ((size_t)n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            idx_t m   = i - 1;                    /* divisors of m = i‑1 */
            seq_t sum = 0;

            /* Enumerate divisors of m. */
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    /* contribution from divisor d */
                    if (d & 1U)
                        sum += cache[d];
                    else
                        sum -= cache[d];

                    /* contribution from the paired divisor m/d */
                    idx_t d2 = m / d;
                    if (d2 != d) {
                        if (d2 & 1U)
                            sum += cache[d2];
                        else
                            sum -= cache[d2];
                    }
                }
            }
            cache[i] = sum;
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}
