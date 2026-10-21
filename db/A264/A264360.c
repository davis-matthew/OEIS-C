/*
 * A264360  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having directed index change 0,0 0,2 1,0 or -1,-2.
 * https://oeis.org/A264360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 28*a(n-2) + 57*a(n-3) + 10*a(n-4) - 24*a(n-5)
 *            + 8*a(n-6) Initial values: a(1)=18, a(2)=120, a(3)=936,
 *            a(4)=7168, a(5)=54720, a(6)=418992 (Sequence A264360)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but defined to avoid UB */

    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of elements cached (size_t) */
    static idx_t cache_size = 0;   /* actual n that has been computed */

    /* Ensure the cache can hold indices up to n */
    if ((size_t)(n + 1) > cache_cap)
        ensure_capacity((size_t)(n + 1), &cache_cap, (void **)&cache, sizeof(seq_t));

    /* If the requested value is already cached, return it directly */
    if (n <= cache_size) return cache[n];

    /* Initialize base cases if the cache is empty or incomplete */
    if (cache_size < 6) {
        /* Start filling from 1 up to at most 6 (or n if smaller) */
        for (idx_t i = cache_size + 1; i <= 6 && i <= n; ++i) {
            switch (i) {
                case 1: cache[1] = (seq_t)18;       break;
                case 2: cache[2] = (seq_t)120;      break;
                case 3: cache[3] = (seq_t)936;      break;
                case 4: cache[4] = (seq_t)7168;     break;
                case 5: cache[5] = (seq_t)54720;    break;
                case 6: cache[6] = (seq_t)418992;   break;
            }
        }
        cache_size = 6;
    }

    /* Compute values for n > cache_size */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t t = cache[i - 1] * (seq_t)3
                + cache[i - 2] * (seq_t)28
                + cache[i - 3] * (seq_t)57
                + cache[i - 4] * (seq_t)10
                + cache[i - 6] * (seq_t)8;   /* add 8*a(n-6) */
        t -= cache[i - 5] * (seq_t)24;      /* subtract 24*a(n-5) */
        cache[i] = t;
    }
    cache_size = n;
    return cache[n];
}
