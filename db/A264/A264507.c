/*
 * A264507  Number of (1+1) X (n+1) arrays of permutations of 0..n*2+1 with
 *          each element having directed index change -1,1 0,-1 0,1 or 1,0.
 * https://oeis.org/A264507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) + a(n-4) + 2*a(n-5) - a(n-6)
 *            (for n ≥ 7). Initial values: a(1)=1, a(2)=1, a(3)=3, a(4)=6,
 *            a(5)=12, a(6)=24
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;         /* array holding a[1 .. cache_n]          */
static size_t cache_cap = 0;        /* allocated capacity of 'cache'          */
static size_t cache_n   = 0;        /* highest index currently computed       */

seq_t a(idx_t n)
{
    if (n < 1)  /* Index lower than the offset */
        return 0;

    /* Ensure base terms are available. */
    if (!cache || cache_cap < 7) {
        size_t needed = 7;                      /* we use indices 1..6   */
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * needed);
        if (!cache) return 0;                   /* allocation failure    */
        cache_cap = needed;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 3;
        cache[4] = 6;
        cache[5] = 12;
        cache[6] = 24;
        cache_n = 6;
    }

    /* Grow the cache if necessary. */
    if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
        if (!tmp) return 0;                      /* allocation failure    */
        cache = tmp;
    }

    /* Compute terms up to n if not already done. */
    if (cache_n < (size_t)n) {
        for (idx_t i = (idx_t)(cache_n + 1); i <= n; ++i) {
            /* Recurrence: 2*a(i-1) + a(i-2) - 3*a(i-3) + a(i-4) + 2*a(i-5) - a(i-6) */
            cache[i] = cache[i-1] * 2;
            cache[i] += cache[i-2];
            cache[i] -= cache[i-3] * 3;
            cache[i] += cache[i-4];
            cache[i] += cache[i-5] * 2;
            cache[i] -= cache[i-6];
        }
        cache_n = (size_t)n;
    }

    return cache[n];
}
