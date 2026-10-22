/*
 * A397858  G.f. A(x) satisfies A(x) = 1 + x * A(x * (1+x)^2).
 * https://oeis.org/A397858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A397858 – a(n) = Sum_{k=0}^{n-1} binomial(2*k, n-k-1) * a(k),  a(0)=1
    The computation uses dynamic programming with memoisation,
    employing the supplied binomial() and ensure_cache() utilities.  */

#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;      /* pointer to the cache array */
    static size_t cap   = 0;         /* current capacity of the array   */
    static idx_t  computed = 0;      /* number of terms already computed */

    /* if n has already been computed, return immediately */
    if (n < computed)
        return cache[n];

    /* make sure the cache can hold at least (n+1) elements */
    ensure_cache((idx_t)(n + 1), &cache, &cap);

    /* base case: a(0) = 1 */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
    }

    /* compute all terms from 'computed' up to n */
    for (idx_t m = computed; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t term = binomial(2*k, m - k - 1);
            term *= cache[k];
            sum += term;
        }
        cache[m] = sum;
    }

    computed = n + 1;
    return cache[n];
}
