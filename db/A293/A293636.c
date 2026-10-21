/*
 * A293636  a(1) = a(2) = 1; a(n) = ( Sum_{i|(n-1)} a(i) ) + Sum_{j|(n-2)}
 *          a(j).
 * https://oeis.org/A293636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i|(n-1)} a(i) + Sum_{j|(n-2)} a(j); a(1)=a(2)=1
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_cache.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* 1‑indexed array */
    static idx_t cap   = 0;       /* allocated capacity */
    static idx_t max_n = 0;       /* largest index computed so far */

    /* Return cached value if already available */
    if (n <= max_n)
        return cache[n - 1];

    /* Grow cache if necessary */
    if (n > cap) {
        seq_t *new_cache = alloc_cache(n);      /* allocate larger array */
        /* copy old values to the new array */
        for (idx_t i = 0; i < max_n; ++i)
            new_cache[i] = cache[i];
        /* free old memory if it was dynamically allocated by alloc_cache */
        /* (alloc_cache may use malloc; if so, free it) */
        #ifdef __cplusplus
            /* In C++ the array is managed by alloc_cache, deallocation is not required */
        #else
            if (cache)
                free(cache);
        #endif
        cache = new_cache;
        cap   = n;
    }

    /* Compute missing terms from max_n+1 up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        if (k == 1 || k == 2) {           /* base cases */
            cache[k - 1] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        idx_t t, d, d2;

        /* Sum over divisors of k-1 */
        t = k - 1;
        for (d = 1; d * d <= t; ++d) {
            if (t % d == 0) {
                sum += cache[d - 1];
                d2 = t / d;
                if (d2 != d)
                    sum += cache[d2 - 1];
            }
        }

        /* Sum over divisors of k-2 */
        t = k - 2;
        for (d = 1; d * d <= t; ++d) {
            if (t % d == 0) {
                sum += cache[d - 1];
                d2 = t / d;
                if (d2 != d)
                    sum += cache[d2 - 1];
            }
        }

        cache[k - 1] = sum;
    }

    max_n = n;
    return cache[n - 1];
}
