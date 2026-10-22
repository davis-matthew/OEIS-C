/*
 * A399846  Binary nested sequence: a(0)=0, a(1)=1, a(2n)=a(n),
 *          a(2n+1)=1-a(n-a(n-1)) for n>=1.
 * https://oeis.org/A399846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2n)=a(n), a(2n+1)= (1-a(n))*(1-a(n-1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Static cache indexed 0 .. last computed value */
    static seq_t *cache = NULL;    /* stored as an array of seq_t */
    static size_t cap   = 0;       /* current capacity of the array    */
    static size_t len   = 0;       /* one past the last valid index    */

    /* On first call, set up cache[0]=0, cache[1]=1 */
    if (cache == NULL) {
        cap = 2;                               /* need indices 0 and 1 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;
    }

    /* If the requested index is beyond what we have, extend the cache */
    if ((size_t)n >= len) {
        const size_t target = (size_t)n + 1;   /* indices 0..n inclusive */

        /* Grow capacity if necessary */
        if (cap < target) {
            size_t new_cap = cap > 0 ? cap << 1 : 1;
            while (new_cap < target) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute all missing terms */
        for (size_t i = len; i < target; ++i) {
            if ((i & 1) == 0) {                /* even index: a(2k) = a(k) */
                cache[i] = cache[i >> 1];
            } else {                          /* odd index: a(2k+1) */
                const size_t k = i >> 1;
                const seq_t v1 = ((seq_t)1) - cache[k];
                const seq_t v2 = ((seq_t)1) - cache[k - 1];
                cache[i] = v1 * v2;           /* (1-a(k))*(1-a(k-1)) */
            }
        }
        len = target;
    }

    return cache[(size_t)n];
}
