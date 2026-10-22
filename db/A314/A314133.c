/*
 * A314133  Coordination sequence Gal.6.623.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5, for n >= 2: a(n) = a(n-1) + 5 if n mod 11 ∈
 *            {2,5,6,7,10} then add an extra 1 (so a(n)-a(n-1)=6); otherwise
 *            a(n)-a(n-1)=5. (Special case: a(1)-a(0)=4.)
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed values */
    static idx_t  cached = 0;     /* highest index computed so far */
    static idx_t  cap    = 0;     /* allocated capacity */

    if (n == 0)                     /* base term */
        return (seq_t)1;

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate at least n+1 entries to compute up to n */
        idx_t init = n + 1;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache)            /* allocation failure */
            return (seq_t)0;
        cache[0] = (seq_t)1;   /* a(0) */
        cap    = init;
        cached = 0;
    }

    /* grow cache if needed */
    if (cached < n) {
        if (cap <= n) {
            idx_t new_cap = cap ? cap * 2 : 16;
            while (new_cap <= n)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)   /* allocation failure */
                return (seq_t)0;
            cache = tmp;
            cap   = new_cap;
        }

        /* compute missing values sequentially */
        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t diff;
            if (i == 1) {                     /* a(1)-a(0) = 4 */
                diff = (seq_t)4;
            } else {
                idx_t m = i % 11;
                if (m == 2 || m == 5 || m == 6 || m == 7 || m == 10)
                    diff = (seq_t)6;
                else
                    diff = (seq_t)5;
            }
            cache[i] = cache[i - 1] + diff;
        }
        cached = n;
    }

    return cache[n];
}
