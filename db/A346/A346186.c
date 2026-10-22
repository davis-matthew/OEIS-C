/*
 * A346186  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n-1,k)^3 * a(k) *
 *          a(n-k-1).
 * https://oeis.org/A346186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} ( binomial(n-1,k)^3 * a(k) *
 *            a(n-1-k) ) for n ≥ 1 (OEIS A346186)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, ensure_cache, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* Static cache to avoid recomputing values.
       The cache holds a(i) for all i ≤ current_max.            */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated slots */
    static idx_t last_computed = 0;   /* highest index already computed */

    /* Ensure the cache can hold indices up to n.           */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* Initialise the zeroth value on first use.            */
    if (last_computed == 0 && (idx_t)0 >= 0) {
        cache[0] = 1;
    }

    /* Compute missing values up to n.                      */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        seq_t sum = 0;               /* running total */

        idx_t upper = (m - 1) / 2;    /* symmetry reduces work          */
        for (idx_t k = 0; k <= upper; ++k) {
            seq_t bi   = binomial(m - 1, k);          /* C(m-1,k)          */
            seq_t bi2  = seq_mul(bi, bi);             /* bi^2              */
            seq_t bi3  = seq_mul(bi2, bi);            /* bi^3              */

            seq_t term = seq_mul(bi3, cache[k]);     /* bi^3 * a(k)       */
            term = seq_mul(term, cache[m - 1 - k]);   /* ... * a(m-1-k)    */

            /* Double the term if k ≠ m-1-k to account for symmetry. */
            if (k != m - 1 - k) {
                term = seq_mul(term, 2);
            }

            sum += term;  /* use +; seq_t supports integer addition */
        }

        cache[m] = sum;
    }

    last_computed = n;
    return cache[n];
}
