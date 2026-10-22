/*
 * A394739  a(1) = 1, a(n) is the number of previous terms that are greater
 *          than half of the previous term.
 * https://oeis.org/A394739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of prior terms that are larger than a(n‑1)/2 with
 *            a(1) = 1
 * Range:     u8 n <= 492, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined for n < 1 */

    /* Memoisation cache for already computed terms */
    static seq_t *cache       = NULL;   /* array of a(1)..a(k) */
    static idx_t cache_len    = 0;      /* number of terms stored (>=0) */
    static idx_t cache_cap    = 0;      /* allocated capacity */

    /* initialise the cache on first call */
    if (cache_len == 0) {
        cache_cap = (idx_t)8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                  /* a(1) = 1 */
        cache_len = (idx_t)1;
    }

    /* compute terms up to n if necessary */
    if (cache_len < n) {
        for (idx_t m = cache_len; m < n; ++m) {
            seq_t prev  = cache[m - 1];

            /* count previous terms > prev/2  <=> 2*term > prev */
            seq_t count = (seq_t)0;
            for (idx_t i = (idx_t)0; i < m; ++i) {
                if ((seq_t)2 * cache[i] > prev) ++count;
            }

            /* grow cache if needed */
            if (m >= cache_cap) {
                idx_t new_cap = cache_cap * (idx_t)2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache); cache = NULL; cache_cap = (idx_t)0;
                    return (seq_t)0;              /* allocation failure */
                }
                cache     = tmp;
                cache_cap = new_cap;
            }

            cache[m] = count;
        }
        cache_len = n;
    }

    return cache[n - (idx_t)1];   /* array is zero‑based */
}
