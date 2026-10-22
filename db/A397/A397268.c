/*
 * A397268  Lower (-1,2) midsequence of (floor[n/2]^2) and (ceiling[n/2]^2);
 *          see Comments.
 * https://oeis.org/A397268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5); initial
 *            values: a(0)=0, a(1)=2, a(2)=1, a(3)=7, a(4)=4
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* stored terms */
static size_t cache_cap = 0;     /* allocated size */
static size_t cache_len = 0;     /* number of terms stored */

seq_t a(idx_t n)
{
    /* Basic initialization on first call */
    if (cache == NULL) {
        cache_cap = 5;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);         /* allocation failure */
        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 1;
        cache[3] = 7;
        cache[4] = 4;
        cache_len = 5;
    }

    /* Grow the cache if needed */
    if ((idx_t)cache_len <= n) {
        if ((idx_t)cache_cap <= n) {
            while ((idx_t)cache_cap <= n) cache_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) exit(1);
            cache = tmp;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term = cache[i-1] + 2 * cache[i-2]
                         - 2 * cache[i-3] - cache[i-4] + cache[i-5];
            cache[i] = term;
        }
        cache_len = n + 1;   /* now we have terms up to a(n) */
    }

    return cache[n];
}
