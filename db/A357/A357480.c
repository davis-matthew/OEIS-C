/*
 * A357480  a(n) = (n!/24) * Sum_{k=0..n-4} 1/k!.
 * https://oeis.org/A357480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * a(n-1) + binomial(n,4) (a(0) = 0; binomial(m,4)=0 for
 *            m<4)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   binom, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include "../utils/ensure_cache.h"   /* ensure_cache(idx_t, seq_t **, size_t *) */
#include "../utils/binom.h"          /* binom(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache      = NULL;   /* array of a(i) values */
    static size_t cache_cap  = 0;      /* current allocated capacity */
    static idx_t  last_done  = 0;      /* smallest index for which cache[i] is invalid */

    /* Compute up to n if needed */
    if (n >= last_done) {
        /* Ensure we can store up to index n */
        ensure_cache(n + 1, &cache, &cache_cap);

        /* Initialise a(0) on first use */
        if (last_done == 0) {
            cache[0] = (seq_t)0;
            last_done = 1;
        }

        /* Compute all new terms */
        for (idx_t i = last_done; i <= n; ++i) {
            seq_t bin = binom(i, 4);                     /* binom(i,4)=0 for i<4 */
            cache[i] = ((seq_t)i) * cache[i - 1] + bin;  /* recurrence */
        }

        last_done = n + 1;  /* mark all up to n as computed */
    }

    return cache[n];
}
