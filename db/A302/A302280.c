/*
 * A302280  Number of 3 X n 0..1 arrays with every element equal to 1, 2 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -4*a(n-2) +4*a(n-3) -7*a(n-4) -17*a(n-5)
 *            +29*a(n-6) -5*a(n-7) +24*a(n-8) +6*a(n-9) -75*a(n-10)
 *            +27*a(n-11) +16*a(n-12) +7*a(n-13) -8*a(n-14) -6*a(n-15)
 *            +6*a(n-16) for n > 18 with initial values: a(1)=0, a(2)=10,
 *            a(3)=22, a(4)=29, a(5)=74, a(6)=162, a(7)=363, a(8)=782,
 *            a(9)=1766, a...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h> /* for memcpy */

static const seq_t INIT[18] = {
    0ULL, 10ULL, 22ULL, 29ULL, 74ULL, 162ULL, 363ULL,
    782ULL, 1766ULL, 3953ULL, 8784ULL, 19606ULL,
    43887ULL, 98136ULL, 219234ULL, 490655ULL,
    1098086ULL, 2457684ULL
};

seq_t a(idx_t n)
{
    /* Return values for the first 18 terms directly. */
    if (n <= 18)
        return INIT[n-1];

    /* Static cache for computed terms, indexed from 1. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n > cache_size) {
        /* Allocate or enlarge the cache. */
        seq_t *tmp = (seq_t *)malloc((size_t)(n+1) * sizeof(seq_t));
        if (!tmp) { /* allocation failed; return 0 as a safe fallback */
            return 0;
        }
        /* Copy existing data if any. */
        if (cache != NULL) {
            memcpy(tmp, cache, (size_t)(cache_size+1) * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        /* Ensure base terms are initialised. */
        if (cache_size < 18) {
            for (idx_t i = 0; i < 18; ++i)
                cache[i+1] = INIT[i];
        }
        /* Compute new terms up to n using the recurrence. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 18) continue;   /* already set */
            seq_t val = 0;
            val += 4ULL * cache[i-1];
            val -= 4ULL * cache[i-2];
            val += 4ULL * cache[i-3];
            val -= 7ULL * cache[i-4];
            val -= 17ULL * cache[i-5];
            val += 29ULL * cache[i-6];
            val -= 5ULL * cache[i-7];
            val += 24ULL * cache[i-8];
            val += 6ULL * cache[i-9];
            val -= 75ULL * cache[i-10];
            val += 27ULL * cache[i-11];
            val += 16ULL * cache[i-12];
            val += 7ULL * cache[i-13];
            val -= 8ULL * cache[i-14];
            val -= 6ULL * cache[i-15];
            val += 6ULL * cache[i-16];
            cache[i] = val;
        }
        cache_size = n;
    }

    return cache[n];
}
