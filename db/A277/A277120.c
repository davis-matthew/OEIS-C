/*
 * A277120  Number of branching factorizations of n.
 * https://oeis.org/A277120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = 1 + Σ_{d|n , 1 < d < n} a(d) *
 *            a(n/d)
 * Range:     u8 n <= 71, u16 n <= 767, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The public function.  The types seq_t and idx_t are supplied by the
   build system.  No assumption is made about the validity of n < 1;
   a(0) simply returns 0 which is safe for the tests. */
seq_t a(idx_t n)
{
    /* Lazy static cache for computed values. */
    static seq_t *cache          = NULL;   /* array indexed by 0.. */
    static size_t cache_cap      = 0;      /* allocated size (in elements)   */
    static idx_t  highest       = 0;       /* largest n that has been computed */

    if (n == 0) return (seq_t)0;           /* safety for out‑of‑bounds calls */

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute all values up to n that have not yet been computed. */
    for (idx_t m = highest + 1; m <= n; ++m) {
        if (m == 1) {
            cache[1] = (seq_t)0;
            continue;
        }

        seq_t sum = (seq_t)1;               /* the +1 term */

        /* Iterate over proper divisors >1 and <m. */
        for (idx_t d = 2; d < m; ++d) {
            if (m % d == 0) {
                idx_t partner = m / d;
                sum += cache[(size_t)d] * cache[(size_t)partner];
            }
        }
        cache[m] = sum;
    }
    highest = n;

    return cache[n];
}
