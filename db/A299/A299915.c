/*
 * A299915  a(n) = A299914(2n).
 * https://oeis.org/A299915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=9*a(n-1)-12*a(n-2) (for n>1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build environment */

seq_t a(idx_t n)
{
    /* Static cache ----------------------------------------------------- */
    static seq_t *cache = NULL;     /* array storing computed values   */
    static idx_t cap = 0;           /* current allocated capacity     */
    static idx_t last = 0;          /* highest index computed in cache */

    /* Initialise cache if it does not yet exist ------------------------ */
    if (cache == NULL) {
        cap = 2;                  /* we need room for a(0) and a(1) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();      /* allocation failure          */
        cache[0] = (seq_t)0;      /* a(0) */
        cache[1] = (seq_t)1;      /* a(1) */
        last   = 1;               /* we have computed up to index 1 */
    }

    /* Grow the cache if the requested index is out of bounds --------- */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) abort();
        for (idx_t i = 0; i < cap; ++i) tmp[i] = cache[i]; /* copy old values */
        free(cache);
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms up to n ----------------------------------- */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /*   a(i) = 9*a(i-1) - 12*a(i-2)  (exact arithmetic in seq_t)   */
            cache[i] = (seq_t)9 * cache[i - 1] - (seq_t)12 * cache[i - 2];
        }
        last = n;
    }

    return cache[n];
}
