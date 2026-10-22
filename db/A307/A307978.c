/*
 * A307978  Expansion of e.g.f. exp((sinh(x) - sin(x))/2).
 * https://oeis.org/A307978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 20, u64 n <= 30, u128 n <= 49,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A307978: a(n) = Sum_{k=0..floor((n-3)/4)} binomial(n-1,4k+2) * a(n-4k-3)
   with a(0)=1, a(1)=0, a(2)=0.  The function memoises previous values so
   calls can arrive in any order. */
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache for all computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;            /* current allocated size (indices 0..cap-1) */
    static idx_t max_computed = (idx_t)-1;  /* highest index with valid value */

    /* Make sure the cache can hold indices up to n */
    if (cache == NULL || n >= cap)
        ensure_cache(n, &cache, &cap);

    /* Compute values that are missing up to n (in any order of calls) */
    if (max_computed == (idx_t)-1) {     /* first use: initialise a(0) */
        cache[0] = 1;
        max_computed = 0;
    }

    for (idx_t m = max_computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        if (m >= 3) {
            idx_t kmax = (m - 3) / 4;
            for (idx_t k = 0; k <= kmax; ++k) {
                idx_t kk = 4 * k + 2;          /* choose position for binomial */
                seq_t binom = binomial(m - 1, kk);
                seq_t term  = binom * cache[m - 4 * k - 3];
                sum += term;
            }
        }
        cache[m] = sum;   /* a(1) and a(2) automatically get 0 */
    }

    max_computed = n;
    return cache[n];
}
