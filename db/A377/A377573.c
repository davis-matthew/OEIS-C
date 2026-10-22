/*
 * A377573  Cogrowth sequence for the 14-element dihedral group D7 = <S,T |
 *          S^7, T^2, (ST)^2>.
 * https://oeis.org/A377573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 2*a(n-4) Initial values:
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=3
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* allocate or grow cache up to index n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {            /* allocation failed – return zero */
            return (seq_t)0;
        }
        cache = tmp;

        /* fill new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)0;
            else if (i == 2) cache[i] = (seq_t)1;
            else if (i == 3) cache[i] = (seq_t)0;
            else if (i == 4) cache[i] = (seq_t)3;
            else {
                cache[i] = cache[i-1]
                         + (seq_t)4 * cache[i-2]
                         - (seq_t)3 * cache[i-3]
                         - (seq_t)2 * cache[i-4];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
