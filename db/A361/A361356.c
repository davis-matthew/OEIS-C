/*
 * A361356  Number of noncrossing caterpillars with n edges.
 * https://oeis.org/A361356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A361356
    Number of noncrossing caterpillars with n edges.
    Recurrence (n ≥ 8):
       a(n) = 12*a(n-1) - 52*a(n-2) + 104*a(n-3) - 114*a(n-4)
              + 76*a(n-5) - 32*a(n-6) + 8*a(n-7) - a(n-8)
    Initial values:
       a(0)=1, a(1)=1, a(2)=3, a(3)=12, a(4)=55, a(5)=273, a(6)=1372, a(7)=6824.
*/

#include <stddef.h>
#include <stdlib.h>

/* External types are provided by the build system */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;

    /* ensure the cache can hold indices up to n */
    size_t need_len = (size_t)n + 1;
    if (need_len < 8)
        need_len = 8;

    /* initialise the cache on first call */
    if (cache_len == 0) {
        cache_len = need_len;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        /* known first eight terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)55;
        cache[5] = (seq_t)273;
        cache[6] = (seq_t)1372;
        cache[7] = (seq_t)6824;
        /* compute any further terms needed on first call */
        for (size_t i = 8; i < cache_len; ++i) {
            seq_t r = 12 * cache[i-1];
            r -= 52 * cache[i-2];
            r += 104 * cache[i-3];
            r -= 114 * cache[i-4];
            r += 76 * cache[i-5];
            r -= 32 * cache[i-6];
            r += 8  * cache[i-7];
            r -= cache[i-8];
            cache[i] = r;
        }
    }

    /* grow the cache if necessary */
    if (need_len > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, need_len * sizeof(seq_t));
        if (!new_cache) return 0;      /* realloc failed */
        cache = new_cache;
        for (size_t i = cache_len; i < need_len; ++i) {
            seq_t r = 12 * cache[i-1];
            r -= 52 * cache[i-2];
            r += 104 * cache[i-3];
            r -= 114 * cache[i-4];
            r += 76 * cache[i-5];
            r -= 32 * cache[i-6];
            r += 8  * cache[i-7];
            r -= cache[i-8];
            cache[i] = r;
        }
        cache_len = need_len;
    }

    return cache[(size_t)n];
}
