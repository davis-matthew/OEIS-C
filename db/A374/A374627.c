/*
 * A374627  Expansion of Product_{k>=0} 1 / (1 - x^(3^k))^3.
 * https://oeis.org/A374627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m=0}^{floor(n/3)} C(n-3m+2, 2) * a(m), a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for OEIS A374627 */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;      /* array of values */
    static idx_t  cache_size = 0;    /* number of computed terms    */
    static size_t cache_cap  = 0;    /* allocated array capacity    */

    /* expand cache if necessary */
    if (n >= cache_size) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp && new_cap > 0)   /* allocation failure */
                return 0;
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* compute terms from cache_size up to n */
        for (idx_t k = cache_size; k <= n; ++k) {
            if (k == 0) {
                cache[0] = 1;
                continue;
            }

            seq_t sum = 0;
            idx_t max_m = k / 3;
            for (idx_t m = 0; m <= max_m; ++m) {
                /* t = k - 3*m + 2 */
                seq_t t = (seq_t)(k - 3*m + 2);
                seq_t coeff = t * (t - 1) / 2;   /* binomial(t,2) */
                sum += cache[m] * coeff;
            }
            cache[k] = sum;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
