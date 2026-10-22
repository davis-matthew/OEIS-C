/*
 * A315418  Coordination sequence Gal.3.21.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) for n > 6; Initial values:
 *            a(0)=1, a(1)=6, a(2)=11, a(3)=15, a(4)=20, a(5)=26, a(6)=32
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;

    /* If the cache is empty, initialise it with the first seven terms. */
    if (cache == NULL) {
        cap = 16;                                 /* start with space for 16 values */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            abort();                               /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)20;
        cache[5] = (seq_t)26;
        cache[6] = (seq_t)32;
        sz = 7;
    }

    /* If requested index is already computed, just return it. */
    if ((size_t)n < sz)
        return cache[n];

    /* Extend the cache up to the requested index using the recurrence. */
    size_t target = (size_t)n;
    while (sz <= target) {
        /* Grow the allocation if necessary. */
        if (sz >= cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL)
                abort();                         /* allocation failure */
            cache = tmp;
            cap = newcap;
        }
        /* Recurrence: a(i) = a(i-1) + a(i-5) - a(i-6) for i > 6. */
        seq_t val = cache[sz - 1] + cache[sz - 5] - cache[sz - 6];
        cache[sz] = val;
        ++sz;
    }

    return cache[n];
}
