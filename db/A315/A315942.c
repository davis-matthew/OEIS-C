/*
 * A315942  Coordination sequence Gal.6.507.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 2*a(n-6) - a(n-12) for n > 12, * with initial values: *
 *            a(0)=1, a(1)=6, a(2)=8, a(3)=18, a(4)=18, a(5)=22, * a(6)=36,
 *            a(7)=34, a(8)=34, a(9)=54, a(10)=44, a(11)=50, * a(12)=72
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_seq[13] = {
    (seq_t)1,  (seq_t)6,  (seq_t)8,  (seq_t)18, (seq_t)18,
    (seq_t)22, (seq_t)36, (seq_t)34, (seq_t)34, (seq_t)54,
    (seq_t)44, (seq_t)50, (seq_t)72
};

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t  *cache   = NULL;   /* array of values */
    static size_t  cap     = 0;      /* allocated capacity */
    static size_t  last    = 0;      /* last computed index */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 13;                     /* initial capacity for a(0)..a(12) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;         /* abort on allocation failure */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init_seq[i];
        last = 12;                   /* last index for which we have values */
    }

    /* if requested index is already in cache, return it */
    if ((size_t)n <= last)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap = new_cap ? new_cap * 2 : 16;   /* exponential grow */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                        /* abort on failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = (idx_t)last + 1; i <= n; ++i) {
        /* i >= 13 because last >=12 at this point */
        cache[i] = (cache[i - 6] << 1) - cache[i - 12];
    }
    last = n;  /* update last computed index */

    return cache[n];
}
