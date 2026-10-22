/*
 * A332792  a(1) = 1; a(n) = Sum_{d|n, d < n} phi(d) * a(d).
 * https://oeis.org/A332792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} phi(d) * a(d) with a(1) = 1. This
 *            implements the recursive definition using memoisation. The
 *            implementation uses the helper seq_realloc to grow the cache so
 *            it works for arbitrary‑precision seq_t types
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 767, u64 n <= 8191,
 *            u128 n <= 9435, big any n
 * Helpers:   calc_phi, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/calc_phi.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset starts at 1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;  /* indices 0 .. n */
        cache = seq_realloc(cache, cache_cap, new_cap);
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        /* initial value of newly added entries is zero */
        for (size_t i = cache_cap; i < (size_t)new_cap; ++i)
            cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* base case */
    if (n == 1) {
        if (cache[1] == (seq_t)0)
            cache[1] = (seq_t)1;
        return cache[1];
    }

    /* return cached value if available */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute the sum over proper divisors */
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if (d < n) {
                seq_t phi_d = calc_phi((seq_t)d);
                sum += phi_d * a(d);
            }
            /* paired divisor n/d */
            idx_t e = n / d;
            if (e != d && e < n) {
                seq_t phi_e = calc_phi((seq_t)e);
                sum += phi_e * a(e);
            }
        }
    }

    cache[n] = sum;
    return sum;
}
