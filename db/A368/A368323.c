/*
 * A368323  Expansion of e.g.f. exp(3*x) / (4 - 3*exp(x)).
 * https://oeis.org/A368323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n + 3 * Sum_{k=1..n} binomial(n,k) * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;               /* number of terms stored (cache[0..cached-1]) */

    if (n >= cached)
    {
        /* enlarge cache */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort */
            abort();
        }
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cached; i <= n; ++i)
        {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t pow3 = pow_uint((uint64_t)3, i);   /* 3^i */
            seq_t sum   = 0;
            seq_t bin   = 1;                          /* C(i,0) */

            for (idx_t k = 1; k <= i; ++k) {
                /* update binomial coefficient C(i,k) */
                bin = bin * ((seq_t)i - (k - 1));
                bin = bin / (seq_t)k;                /* exact division */

                /* term = 3 * bin * a[i-k] */
                seq_t term = bin * cache[i - k];
                term = term * (seq_t)3;
                sum += term;
            }

            cache[i] = pow3 + sum;
        }
        cached = new_size;
    }

    return cache[n];
}
