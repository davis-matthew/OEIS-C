/*
 * A255226  Number of (n+2)X(6+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 0 and no antidiagonal sum 0 and no row sum 2 and no column sum
 *          2.
 * https://oeis.org/A255226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) Initial values:
 *            a(1)=88, a(2)=94, a(3)=103, a(4)=115
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based array stored 0‑based   */
static idx_t  cache_len = 0;    /* number of elements stored     */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    if (n == 0)
        return 0; /* undefined for 0, return 0 as fallback */

    /* initialize cache with first four terms */
    if (!cache) {
        const idx_t init_size = 4;
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure, return 0 as safe fallback */
        cache[0] = 88;  /* a(1) */
        cache[1] = 94;  /* a(2) */
        cache[2] = 103; /* a(3) */
        cache[3] = 115; /* a(4) */
        cache_len = init_size;
    }

    /* extend cache if needed */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            return 0; /* allocation failure, return 0 as safe fallback */
        cache = new_cache;

        /* compute new terms using recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            /* recurrence: a(i+1) = 2*a(i) - a(i-1) + a(i-2) - a(i-3) */
            cache[i] = 2 * cache[i-1]
                       - cache[i-2]
                       + cache[i-3]
                       - cache[i-4];
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
