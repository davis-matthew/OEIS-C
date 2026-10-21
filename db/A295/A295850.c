/*
 * A295850  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = 0, a(2) = 2, a(3) = 1.
 * https://oeis.org/A295850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) with initial
 *            values: a(0) = 0, a(1) = 0, a(2) = 2, a(3) = 1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms; capacity in terms of indices   */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;

    /* initialise cache on first call; store first four terms   */
    if (cap == 0) {
        cap = 4;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();          /* program failure on memory error */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
    }

    /* if the requested index is already cached, just return it   */
    if (n < cap)
        return cache[n];

    /* need to grow the cache so that it can hold index n          */
    idx_t new_cap = cap;
    while (n >= new_cap)            /* double until capacity is enough */
        new_cap <<= 1;

    /* remember the old capacity so we know where to start filling  */
    idx_t old_cap = cap;

    /* perform the reallocation; fail on memory error              */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        free(cache);                 /* terminate program if we cannot grow */
        abort();
    }
    cache = tmp;

    /* compute all missing terms up to the new capacity            */
    for (idx_t i = old_cap; i < new_cap; ++i) {
        /* compute a(i) = (a(i-1) + 3*a(i-2)) - (2*a(i-3) + 2*a(i-4))   */
        seq_t term1 = cache[i-1];
        seq_t term2 = (seq_t)3 * cache[i-2];
        seq_t sub   = (seq_t)2 * cache[i-3] + (seq_t)2 * cache[i-4];
        cache[i] = term1 + term2 - sub;
    }

    cap = new_cap;
    return cache[n];
}
