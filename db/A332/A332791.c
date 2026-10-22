/*
 * A332791  a(1) = 1; a(n+1) = Sum_{d|n} phi(d) * a(d).
 * https://oeis.org/A332791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n+1)=Σ_{d|n} φ(d)*a(d) (OEIS A332791)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 25, u128 n <= 40,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    /* cache[0] is unused; cache[1] = a(1) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of allocated slots (including index 0) */

    /* Ensure there is space for index n */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1; /* allow index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed */
            /* In this framework we assume allocation is always successful. */
            return (seq_t)0;
        }
        /* initialize newly allocated entries to zero */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* a(1) = 1 */
    if (n == 1) {
        if (cache[1] == (seq_t)0)  /* store only once */
            cache[1] = (seq_t)1;
        return cache[1];
    }

    /* Return memoized value if available */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* Compute a(n) = Σ_{d|n-1} φ(d) * a(d) */
    idx_t m = n - 1;
    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            /* first divisor */
            idx_t d1 = i;
            seq_t term1 = calc_phi((seq_t)d1) * a(d1);
            sum += term1;

            /* paired divisor */
            idx_t d2 = m / i;
            if (d2 != d1) {
                seq_t term2 = calc_phi((seq_t)d2) * a(d2);
                sum += term2;
            }
        }
    }

    cache[n] = sum;
    return sum;
}
