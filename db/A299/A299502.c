/*
 * A299502  Expansion of (1 - 6*x + x^2 + 8*x^3 + 16*x^4)^(-1/2).
 * https://oeis.org/A299502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A299502: d-finite recurrence
   (16*n-32)*a(n-4) + (8*n-12)*a(n-3) + (n-1)*a(n-2) + (3-6*n)*a(n-1) + n*a(n) = 0
   Initial values: a(0)=1, a(1)=3, a(2)=13, a(3)=59
   The function memoises computed terms in a static array so that calls in
   arbitrary order remain efficient. */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* trivial direct lookup for the first four terms */
    if (n < 4) {
        static const seq_t init[] = { (seq_t)1, (seq_t)3, (seq_t)13, (seq_t)59 };
        return init[n];
    }

    /* static cache */
    static seq_t *cache = NULL;
    static idx_t  cache_cap   = 0;   /* allocated size   */
    static idx_t  cache_last  = 0;   /* largest index computed */

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache_cap  = 4;
        cache      = seq_realloc(NULL, 0, cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)59;
        cache_last = 3;
    }

    /* grow cache if we need more space */
    if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        cache = seq_realloc(cache, cache_last + 1, cache_cap);
    }

    /* compute missing values up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* recurrence derived from the D-finite relation (see comment above) */
        seq_t t1 = (seq_t)(16 * i - 32) * cache[i - 4];
        seq_t t2 = (seq_t)(8  * i - 12) * cache[i - 3];
        seq_t t3 = (seq_t)(i - 1)      * cache[i - 2];
        seq_t t4 = (seq_t)(6  * i - 3)  * cache[i - 1];
        seq_t numer = t4 - t1 - t2 - t3;          /* always non-negative for n>=4 */
        cache[i] = numer / (seq_t)i;
    }

    cache_last = n;
    return cache[n];
}
