/*
 * A292291  Number of vertices of type B at level n of the hyperbolic Pascal
 *          pyramid.
 * https://oeis.org/A292291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) with initial values a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic memoisation buffer   */
    static idx_t cache_len = 0;   /* number of values already stored */

    /* Return immediately if we already know a(n) */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate up to a(n) */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp)                /* out‑of‑memory – abort gracefully */
        return 0;

    cache = tmp;

    /* Compute any new values that are not yet cached */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i == 0 || i == 1 || i == 2) {
            cache[i] = 0;
        } else if (i == 3) {
            cache[i] = 3;
        } else {                       /* recurrence */
            seq_t t1 = 4 * cache[i - 1];
            seq_t t2 = 4 * cache[i - 2];
            cache[i] = t1 - t2 + cache[i - 3];
        }
    }

    cache_len = new_len;
    return cache[n];
}
