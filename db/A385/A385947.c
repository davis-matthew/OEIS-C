/*
 * A385947  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(k+5,5) * binomial(n-1,k)
 *          * a(k) * a(n-1-k).
 * https://oeis.org/A385947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A385947
   a(0) = 1;
   a(n) = Sum_{k=0..n-1} binomial(k+5,5) * binomial(n-1,k) * a(k) * a(n-1-k) */

#include <stddef.h>
#include "../utils/alloc_cache.h"   /* seq_t *alloc_cache(idx_t n); */
#include "../utils/free_cache.h"    /* void free_cache(seq_t *p); */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* allocate or enlarge cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = alloc_cache(new_cap);
        /* copy existing values */
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_cap; i++)
                new_cache[i] = cache[i];
        }
        /* initialise new slots to 0 */
        for (idx_t i = cache_cap; i < new_cap; i++)
            new_cache[i] = 0;
        /* free old cache */
        if (cache != NULL)
            free_cache(cache);
        cache = new_cache;
        cache_cap = new_cap;
        /* initialise first element */
        if (cache_cap > 0 && cache[0] == 0)
            cache[0] = 1;
    }

    /* compute terms up to n */
    for (idx_t m = 1; m <= n; ++m) {
        if (cache[m] != 0)          /* already computed */
            continue;

        seq_t sum = 0;
        seq_t bin_nk = 1;          /* C(m-1,0) */
        seq_t bin_k5 = 1;          /* C(0+5,5) */

        for (idx_t k = 0; k < m; ++k) {
            /* term = a(k) * a(m-1-k) * C(k+5,5) * C(m-1,k) */
            seq_t term = cache[k];
            term *= cache[m - 1 - k];
            term *= bin_k5;
            term *= bin_nk;
            sum += term;

            if (k + 1 < m) {
                /* update bin_k5: C(k+6,5) = C(k+5,5) * (k+6)/(k+1) */
                bin_k5 *= (seq_t)(k + 6);
                bin_k5 /= (seq_t)(k + 1);
                /* update bin_nk: C(m-1,k+1) = C(m-1,k) * (m-1-k)/(k+1) */
                bin_nk *= (seq_t)(m - 1 - k);
                bin_nk /= (seq_t)(k + 1);
            }
        }
        cache[m] = sum;
    }

    return cache[n];
}
