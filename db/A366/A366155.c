/*
 * A366155  Expansion of e.g.f. exp(x^3/(3*(1-x)^3)).
 * https://oeis.org/A366155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(n-1)*a(n-1) - 6*(n-2)*(n-1)*a(n-2) * +
 *            (n-2)*(n-1)*(4*n-11)*a(n-3) * - (n-4)*(n-3)*(n-2)*(n-1)*a(n-4) *
 *            * Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=2, a(4)=24
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;          /* array of computed terms   */
    static idx_t cache_cap = 0;          /* current allocated size    */
    static idx_t cache_size = 0;         /* number of terms stored    */

    /* initialise storage if needed */
    if (cache == NULL) {
        /* Capacity at least 8, with the first 5 terms known */
        cache_cap  = 8;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */

        /* Known terms up to a(4) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)24;
        cache_size = 5;                  /* we have a(0)..a(4)   */
    }

    /* if request is within the currently computed range */
    if (n < cache_size)
        return cache[n];

    /* Ensure capacity covers up to index n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* terms needed are defined for i >= 5 only */
        seq_t term1 = cache[i - 1] * (seq_t)(4 * (i - 1));
        seq_t term2 = cache[i - 2] * (seq_t)((i - 2) * (i - 1) * 6);
        seq_t term3 = cache[i - 3] *
                      (seq_t)((i - 2) * (i - 1) * (4 * i - 11));
        seq_t term4 = cache[i - 4] *
                      (seq_t)((i - 4) * (i - 3) * (i - 2) * (i - 1));

        cache[i] = term1 - term2 + term3 - term4;
    }

    cache_size = n + 1;   /* now all values up to a(n) are stored */
    return cache[n];
}
