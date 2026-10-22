/*
 * A345458  a(n) = Sum_{k=0..n} binomial(5*n+4,5*k).
 * https://oeis.org/A345458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=127, a(2)=3004, a(n)=21*a(n-1)+353*a(n-2)-32*a(n-3)
 *            (n>2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static memoisation cache */
static seq_t *cache      = NULL;   /* array of computed a(n) values   */
static size_t cache_len  = 0;      /* number of values stored         */
static size_t cache_cap  = 0;      /* allocated capacity of cache     */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 3;                                  /* at least entries for n = 0..2 */
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                           /* allocation failure */
        cache[0] = 1;                                   /* a(0) */
        cache[1] = 127;                                 /* a(1) */
        cache[2] = 3004;                                /* a(2) */
        cache_len = 3;                                  /* values 0,1,2 known */
    }

    /* if already computed, return cached value */
    if (n < (idx_t)cache_len)
        return cache[n];

    /* enlarge cache if required to hold up to index n */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) {
            new_cap = (new_cap == 0) ? 4 : new_cap * 2;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;         /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values with the recurrence */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = 127;
        } else if (i == 2) {
            cache[i] = 3004;
        } else {
            cache[i] = (seq_t)((seq_t)21 * cache[i-1]
                               + (seq_t)353 * cache[i-2]
                               - (seq_t)32  * cache[i-3]);
        }
    }

    cache_len = n + 1;          /* now all 0..n are stored */
    return cache[n];
}
