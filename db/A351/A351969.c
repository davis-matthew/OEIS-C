/*
 * A351969  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/4)} binomial(n-1,4*k) *
 *          a(k) * a(n-4*k-1).
 * https://oeis.org/A351969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A351969
   Recurrence:
     a(0) = 1
     a(n) = Σ_{k=0..⌊(n-1)/4⌋} binomial(n-1,4k) * a(k) * a(n-4k-1)
*/

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;

    /* initialise cache with a(0) = 1 */
    if (!cache) {
        cache = (seq_t*)calloc(1, sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure – fallback */
        cache[0] = (seq_t)1;
        cap   = 1;
        size  = 1;
    }

    /* grow cache if necessary */
    if (n >= size) {
        size_t newcap = (cap > 0) ? cap : 1;
        while (newcap <= n) newcap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure – fallback */
        cache = tmp;
        cap   = newcap;

        /* compute missing terms up to n */
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) continue;  /* already set */

            seq_t sum = 0;
            idx_t maxk = (i - 1) / 4;
            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t bin = binom((idx_t)(i - 1), 4 * k);
                seq_t prod = bin * cache[k] * cache[i - 4 * k - 1];
                sum += prod;
            }
            cache[i] = sum;
        }
        size = n + 1;
    }

    return cache[n];
}
