/*
 * A258560  Number of (7+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 3*a(n-5) +
 *            3*a(n-6) - a(n-7) for n > 14, with a(1)=2008, a(2)=4667,
 *            a(3)=6348, a(4)=9688, a(5)=12940, a(6)=16942, a(7)=21653,
 *            a(8)=26628, a(9)=31806, a(10)=37351, a(11)=43158, a(12)=49004,
 *            a(13)=54998, a(14)=61394
 * Range:     u8 none, u16 n <= 14, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but safe for non‑positive indices */

    static seq_t *cache = NULL;       /* 1‑based cache; cache[0] unused */
    static idx_t cache_len = 0;       /* number of slots allocated (including index 0) */

    /* Initial values for n = 1 … 14 */
    static const seq_t init[14] = {
        2008, 4667, 6348, 9688, 12940,
        16942, 21653, 26628, 31806, 37351,
        43158, 49004, 54998, 61394
    };

    /* Allocate cache if needed */
    if (cache == NULL) {
        /* Allocate space for indices 0 … 14 (15 elements) */
        cache_len = 15;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;                      /* unused dummy slot */
        for (idx_t i = 1; i <= 14; i++)
            cache[i] = init[i - 1];
    }
    else if (n >= cache_len) {
        /* Grow cache to accommodate index n */
        idx_t old_len = cache_len;
        cache_len = n + 1;                /* +1 to allow 1‑based indexing */
        seq_t *tmp = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        cache = tmp;
        /* Compute any new terms beyond the old cache size, up to n */
        for (idx_t i = old_len; i <= n; ++i) {
            /* Only compute via recurrence for i > 14 */
            if (i <= 14) {
                cache[i] = init[i - 1];
                continue;
            }
            /* Big = 3*a(n-1) + a(n-3)+a(n-4)+3*a(n-6) */
            seq_t big = 3 * cache[i - 1] + cache[i - 3] + cache[i - 4] + 3 * cache[i - 6];
            /* Small = 3*a(n-2)+3*a(n-5)+a(n-7) */
            seq_t small = 3 * cache[i - 2] + 3 * cache[i - 5] + cache[i - 7];
            cache[i] = big - small;        /* guaranteed non‑negative */
        }
        return cache[n];
    }

    /* Cache already large enough; compute missing terms if any */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 14) {
            cache[i] = init[i - 1];
            continue;
        }
        seq_t big = 3 * cache[i - 1] + cache[i - 3] + cache[i - 4] + 3 * cache[i - 6];
        seq_t small = 3 * cache[i - 2] + 3 * cache[i - 5] + cache[i - 7];
        cache[i] = big - small;
    }
    return cache[n];
}
