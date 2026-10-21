/*
 * A252382  Number of (n+2) X (6+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 3 5 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 7.
 * https://oeis.org/A252382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-3) - 4*a(n-6) + a(n-9) for n>12 Initial values:
 *            a(1)=4682 , a(2)=1496 , a(3)=1341 , a(4)=1475 , a(5)=1894 ,
 *            a(6)=2356 , a(7)=2778 , a(8)=3836 , a(9)=5035 , a(10)=6189 ,
 *            a(11)=8920 , a(12)=12049 The function caches the sequence so
 *            that any n≥1 can be computed in O(n) time overall, wit...
 * Range:     u8 none, u16 n <= 17, u32 n <= 52, u64 n <= 121, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h> /* for uintptr_t conversion if needed */

seq_t a(idx_t n)
{
    /* The sequence defined only for n ≥ 1; return 0 for n == 0 to avoid UB */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* actual allocated size (in elements) */
    static idx_t cache_len = 0;    /* highest index computed so far (>=0) */

    /* Expand the cache if necessary */
    if (n > cache_len) {
        /* Allocate or grow the array to hold indices 0 … n */
        size_t needed = (size_t)(n + 1);          /* include 0 index */
        if (needed > cache_cap) {
            if (cache == NULL) {
                cache = (seq_t *)malloc(needed * sizeof(seq_t));
            } else {
                seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
                /* realloc may fail; in that case leave cache untouched,
                   allowing the function to retry (unlikely for small n). */
                if (tmp != NULL) {
                    cache = tmp;
                }
            }
            if (cache == NULL) {
                /* Allocation failed; fall back to zero (should never happen). */
                return (seq_t)0;
            }
            cache_cap = needed;
        }

        /* Initial known values (cache[0] unused) */
        static const seq_t init[13] = {
            0ULL,        /* dummy a(0) */
            4682ULL, 1496ULL, 1341ULL, 1475ULL,
            1894ULL, 2356ULL, 2778ULL, 3836ULL,
            5035ULL, 6189ULL, 8920ULL, 12049ULL
        };

        /* Compute new terms up to index n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 12) {
                cache[i] = init[(size_t)i];
            } else {
                seq_t t1 = cache[i - 3];
                t1 = t1 * (seq_t)4;
                seq_t t2 = cache[i - 6];
                t2 = t2 * (seq_t)4;
                cache[i] = t1 - t2 + cache[i - 9];
            }
        }
        cache_len = n;
    }

    return cache[n];
}
