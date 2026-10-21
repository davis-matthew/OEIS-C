/*
 * A295938  Number of n X 3 0..1 arrays with each 1 adjacent to 1 or 3
 *          king-move neighboring 1's.
 * https://oeis.org/A295938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 18*a(n-3) + a(n-4) - 19*a(n-5) -
 *            36*a(n-6) - 13*a(n-7) + 5*a(n-8) + 10*a(n-9) Initial values (n =
 *            1..9): 3, 15, 41, 120, 465, 1472, 4667, 16230, 53266 (Offset 1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is outside the offset; return 0 */
    if (n == 0)
        return 0;

    /* Static cache for memoisation.
       cache[k] stores a(k+1); cache_sz counts stored terms. */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;

    /* Return previously cached value if available */
    if (n <= cache_sz)
        return cache[n - 1];

    /* Grow the cache to cover n */
    idx_t new_sz = n;
    cache = (seq_t *)realloc(cache, (size_t)new_sz * sizeof(seq_t));

    /* Initial nine terms */
    static const seq_t init[9] = {
        (seq_t)3,      (seq_t)15,     (seq_t)41,
        (seq_t)120,    (seq_t)465,    (seq_t)1472,
        (seq_t)4667,   (seq_t)16230,  (seq_t)53266
    };

    /* Compute terms that are not yet cached */
    for (idx_t i = cache_sz; i < new_sz; ++i) {
        if (i < 9) {
            /* i indexes a(i+1) */
            cache[i] = init[i];
        } else {
            /* Recurrence for a(i+1) where i >= 9 */
            seq_t pos = cache[i - 1]          /* a(n-1) */
                        + 3 * cache[i - 2]    /* a(n-2) */
                        + 18 * cache[i - 3]   /* a(n-3) */
                        + cache[i - 4]        /* a(n-4) */
                        + 5 * cache[i - 8]    /* a(n-8) */
                        + 10 * cache[i - 9];  /* a(n-9) */
            seq_t neg = 19 * cache[i - 5]     /* a(n-5) */
                        + 36 * cache[i - 6]   /* a(n-6) */
                        + 13 * cache[i - 7];  /* a(n-7) */
            cache[i] = pos - neg;
        }
    }

    cache_sz = new_sz;
    return cache[n - 1];
}
