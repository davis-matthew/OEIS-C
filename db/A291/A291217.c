/*
 * A291217  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S^3.
 * https://oeis.org/A291217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 49, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A291217
   Recurrence: a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=3, a(5)=1, a(6)=6,
                a(n)=3*a(n-2)+a(n-3)-3*a(n-4)+a(n-6) for n≥7
*/
#include <stdlib.h>

/* Cached sequence values: indices 0 .. cache_len-1 are valid. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* initialise cache with first 7 terms */
    if (!cache) {
        cache_cap = 7;
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;  /* a(0) */
        cache[1] = 0;  /* a(1) */
        cache[2] = 1;  /* a(2) */
        cache[3] = 0;  /* a(3) */
        cache[4] = 3;  /* a(4) */
        cache[5] = 1;  /* a(5) */
        cache[6] = 6;  /* a(6) */
        cache_len = 7;
    }

    /* extend cache if necessary */
    if ((idx_t)cache_len <= n) {
        size_t need = (size_t)n + 1;
        if (cache_cap < need) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 8;
            if (new_cap < need) new_cap = need;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* compute missing terms using recurrence */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            size_t idx = (size_t)i;
            if (i < 7) {
                /* already initialized */
                continue;
            }
            seq_t sum = ((seq_t)3) * cache[idx - 2];
            sum += cache[idx - 3];
            sum += cache[idx - 6];
            seq_t sub = ((seq_t)3) * cache[idx - 4];
            cache[idx] = sum - sub;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
