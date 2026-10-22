/*
 * A367925  Expansion of e.g.f. 1/(4 - x - 3*exp(x)).
 * https://oeis.org/A367925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + 3 * Sum_{j=0..n-1} binom(n,j) * a(j), a(0) = 1
 *            (recurrence from the e.g.f. 1/(4 – x – 3*exp(x)) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* stored a(k) for 0 <= k <= cache_size-1 */
    static idx_t cache_size = 0;     /* number of cached terms */

    if (n + 1 > cache_size) {        /* need to compute new terms */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;

        /* initialise first term if the cache was empty */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;     /* a(0) = 1 */
            cache_size = 1;
        }

        /* compute a(m) for m from cache_size up to n */
        for (idx_t m = cache_size; m <= n; ++m) {
            if (m == 0) {            /* already initialized */
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum = 0;
            seq_t binom = 1;          /* binom(m,0) */
            for (idx_t j = 0; j < m; ++j) {
                /* add binom(m,j) * a(j) to the sum */
                sum += binom * cache[j];

                /* update binom to binom(m,j+1) */
                seq_t prod = binom * (seq_t)(m - j);   /* binom * (m-j) */
                binom = prod / (seq_t)(j + 1);         /* exact division */
            }

            cache[m] = (seq_t)m * cache[m - 1] + (seq_t)3 * sum;
        }

        cache_size = n + 1;            /* all new terms are cached */
    }

    return cache[n];
}
