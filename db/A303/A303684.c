/*
 * A303684  Number of nX2 0..1 arrays with every element unequal to 2, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 2*a(n-3) - 2*a(n-4) - 4*a(n-5) (for n
 *            > 6; initial values: a(1)=0, a(2)=3, a(3)=5, a(4)=18, a(5)=61,
 *            a(6)=209 )
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache to avoid recomputing previous terms; 
       cache[i] holds a(i+1).  Indices are 0‑based. */
    static seq_t *cache              = NULL;
    static idx_t  cached_size       = 0;   /* number of terms cached */
    static size_t cache_capacity    = 0;   /* allocated slots */

    /* If n is outside the natural range, return 0 (behavior below offset
       does not matter). */
    if (n == 0) return 0;

    /* Ensure the cache has room for n terms. */
    if (cache_capacity < n) {
        size_t new_cap = cache_capacity > 0 ? cache_capacity * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Populate the cache up to term n if needed. */
    if (cached_size < n) {
        /* Base values for a(1)..a(6).  Cache indices 0..5. */
        static const seq_t base[6] = {0, 3, 5, 18, 61, 209};

        for (idx_t i = cached_size + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i - 1] = base[i - 1];
            } else {
                /* a(i) = 3*a(i-1) + a(i-2) + 2*a(i-3)
                           - 2*a(i-4) - 4*a(i-5)              */
                cache[i - 1] = (3 * cache[i - 2] + cache[i - 3] + 2 * cache[i - 4])
                               - (2 * cache[i - 5] + 4 * cache[i - 6]);
            }
        }
        cached_size = n;
    }

    return cache[n - 1];
}
