/*
 * A252380  Number of (n+2) X (4+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 3 5 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 7.
 * https://oeis.org/A252380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-3) - 4*a(n-6) + a(n-9) (n > 12) * Initial values
 *            for 1‑indexed indices: * a(1)=1966, a(2)=771, a(3)=663,
 *            a(4)=744, * a(5)=916, a(6)=1097, a(7)=1361, a(8)=1791, *
 *            a(9)=2270, a(10)=2976, a(11)=4082, a(12)=5341 * * The function
 *            uses a static memoisation cache that grows * as needed. It...
 * Range:     u8 none, u16 n <= 19, u32 n <= 54, u64 n <= 123, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* largest index currently cached (0 means none) */

    /* initialise cache with the known first 12 values (indices 0..12) */
    if (!cache) {
        cache = (seq_t *)realloc(NULL, (12 + 1) * sizeof(seq_t));
        cache[0]  = (seq_t)0;          /* unused but convenient for 1‑based indexing */
        cache[1]  = (seq_t)1966;
        cache[2]  = (seq_t)771;
        cache[3]  = (seq_t)663;
        cache[4]  = (seq_t)744;
        cache[5]  = (seq_t)916;
        cache[6]  = (seq_t)1097;
        cache[7]  = (seq_t)1361;
        cache[8]  = (seq_t)1791;
        cache[9]  = (seq_t)2270;
        cache[10] = (seq_t)2976;
        cache[11] = (seq_t)4082;
        cache[12] = (seq_t)5341;
        cache_len = 12;
    }

    /* if requested n is already cached, return it immediately */
    if (n <= cache_len)
        return cache[n];

    /* enlarge the cache to hold indices up to n (inclusive) */
    if (n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;
        /* compute the missing terms using the recurrence relation */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* i > 12, so i-3, i-6, i-9 are guaranteed to be valid indices */
            seq_t term = (seq_t)4 * cache[i - 3]
                       - (seq_t)4 * cache[i - 6]
                       +       cache[i - 9];
            cache[i] = term;
        }
        cache_len = n;
    }

    return cache[n];
}
