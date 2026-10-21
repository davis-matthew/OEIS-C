/*
 * A264239  Number of (n+1) X (3+1) arrays of permutations of 0..n*4+3 with
 *          each element having directed index change 0,0 1,1 0,-1 -1,1 or
 *          0,-2.
 * https://oeis.org/A264239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 7*a(n-2) - 65*a(n-3) + 65*a(n-4) + 62*a(n-5) -
 *            72*a(n-6), with a(1)=21, a(2)=95, a(3)=804, a(4)=5055,
 *            a(5)=36193, a(6)=243881
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>               /* for size_t */
#include "../utils/seq_realloc.h"

/* Memoised recurrence implementation */
static seq_t *cache   = NULL;   /* array of computed values */
static idx_t cache_sz = 0;      /* number of values stored (cache[0] = a(1)) */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined but harmless */

    /* Grow the cache if needed. */
    if (n > cache_sz) {
        seq_t *new_cache = seq_realloc(cache, cache_sz, (size_t)n);
        if (!new_cache) return 0;        /* allocation failed */
        cache = new_cache;

        /* Compute new terms up to n. */
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            if (i == 1)         cache[i-1] = (seq_t)21;
            else if (i == 2)    cache[i-1] = (seq_t)95;
            else if (i == 3)    cache[i-1] = (seq_t)804;
            else if (i == 4)    cache[i-1] = (seq_t)5055;
            else if (i == 5)    cache[i-1] = (seq_t)36193;
            else if (i == 6)    cache[i-1] = (seq_t)243881;
            else {
                seq_t a1 = cache[i-2]; /* a(i-1) */
                seq_t a2 = cache[i-3]; /* a(i-2) */
                seq_t a3 = cache[i-4]; /* a(i-3) */
                seq_t a4 = cache[i-5]; /* a(i-4) */
                seq_t a5 = cache[i-6]; /* a(i-5) */
                seq_t a6 = cache[i-7]; /* a(i-6) */
                cache[i-1] = 7*a1 + 7*a2 - 65*a3 + 65*a4 + 62*a5 - 72*a6;
            }
        }
        cache_sz = n;
    }
    return cache[n-1];
}
