/*
 * A310394  Coordination sequence Gal.6.568.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-7) - a(n-14) for n > 14 initial values: a(0)=1,
 *            a(1)=4, a(2)=10, a(3)=14, a(4)=19, a(5)=23, a(6)=29, a(7)=33,
 *            a(8)=37, a(9)=44, a(10)=47, a(11)=52, a(12)=56, a(13)=62,
 *            a(14)=66
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache and its current size */
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;

    /* if value already cached, return immediately */
    if (n < cache_sz)
        return cache[n];

    /* allocate enough space for indices [0..n] */
    size_t new_sz = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;

    /* initial values for indices 0..14 */
    static const seq_t init[15] = {
        1, 4, 10, 14, 19, 23, 29, 33, 37, 44, 47, 52, 56, 62, 66
    };

    /* compute any missing entries, from old cache_sz up to n */
    for (size_t i = cache_sz; i < new_sz; ++i) {
        if (i <= 14) {
            cache[i] = init[i];
        } else {
            /* recurrence: a(i) = 2*a(i-7) - a(i-14) */
            cache[i] = 2 * cache[i - 7] - cache[i - 14];
        }
    }

    cache_sz = new_sz;
    return cache[n];
}
