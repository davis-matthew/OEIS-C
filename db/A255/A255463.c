/*
 * A255463  a(n) = 3*4^n - 2*3^n.
 * https://oeis.org/A255463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*4^n - 2*3^n Recurrence: a(0)=1, a(1)=6,
 *            a(n)=7*a(n-1)-12*a(n-2) for n≥2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache of computed values       */
    static seq_t *cache = NULL;     /* array of a[0..cache_len-1]       */
    static idx_t cache_len = 0;     /* number of valid entries          */

    if (n >= cache_len) {
        /* extend cache to size n+1 */
        idx_t new_len = n + 1;
        /* Preserve old contents and enlarge */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {            /* allocation failed; fall back to no cache */
            if (cache) free(cache);
            cache = NULL;
            cache_len = 0;
            /* fall through to compute directly (very unlikely) */
        } else {
            cache = tmp;
            /* initialise new entries */
            if (cache_len == 0) {
                cache[0] = (seq_t)1;
                cache_len = 1;
            }
            if (cache_len <= 1 && n >= 1) {
                cache[1] = (seq_t)6;
                cache_len = 2;
            }
            /* compute from the first index not yet known */
            for (idx_t i = cache_len; i <= n; ++i) {
                /* a(i) = 7*a(i-1) - 12*a(i-2)   (valid for i≥2) */
                cache[i] = (seq_t)7 * cache[i - 1] - (seq_t)12 * cache[i - 2];
            }
            cache_len = new_len;
        }
    }

    /* if allocation failed in the above block, compute directly */
    if (!cache) {
        /* use the closed‑form expression to avoid repeated addition */
        seq_t pow4 = 1, pow3 = 1;
        for (idx_t i = 0; i < n; ++i) {
            pow4 *= (seq_t)4;
            pow3 *= (seq_t)3;
        }
        return (seq_t)3 * pow4 - (seq_t)2 * pow3;
    }

    return cache[n];
}
