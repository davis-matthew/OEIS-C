/*
 * A336114  The hafnian of a symmetric Toeplitz matrix of order 2*n, n>=2 with
 *          the first row (0,1,2,...,2,1); a(0)=a(1)=1.
 * https://oeis.org/A336114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 6, a(3) = 64, a(4) = 930 a(n+1) =
 *            (4*n+3)*a(n) - (4*n-7)*a(n-1) - a(n-2), for n >= 4 (OEIS
 *            A336114)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* 1. Cache of previously computed terms. */
    static seq_t *cache     = NULL;
    static idx_t  cache_len = 0;   /* number of computed entries          */
    static idx_t  cache_cap = 0;   /* allocated capacity in elements       */

    /* 2. Ensure the cache can hold a(n). */
    idx_t need = n + 1;            /* we need indices 0 .. n inclusive    */
    if (cache_cap < need) {
        idx_t new_cap = need * 2;
        if (new_cap < need) new_cap = need;           /* safety for overflow   */
        seq_t *p;
        if (cache == NULL)
            p = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) { /* allocation failure: return 0 as error value   */
            return (seq_t)0;
        }
        cache      = p;
        cache_cap  = new_cap;
    }

    /* 3. Compute any missing terms up to a(n). */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)6;
        } else if (i == 3) {
            cache[i] = (seq_t)64;
        } else if (i == 4) {
            cache[i] = (seq_t)930;
        } else {
            /* Recurrence: a(i) = (4*(i-1)+3)*a(i-1) -
                                 (4*(i-1)-7)*a(i-2) - a(i-3)   */
            seq_t term1 = ((seq_t)(4 * (i - 1) + 3)) * cache[i - 1];
            seq_t term2 = ((seq_t)(4 * (i - 1) - 7)) * cache[i - 2];
            cache[i] = term1 - term2 - cache[i - 3];
        }
    }
    cache_len = n + 1;  /* we have now computed indices 0 .. n          */

    return cache[n];
}
