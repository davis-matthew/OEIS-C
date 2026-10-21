/*
 * A261956  Start with a single equilateral triangle for n=0; for the odd n-th
 *          generation add a triangle at each expandable vertex of the
 *          triangles of the (n-1)-th generation (this is the "side to vertex"
 *          version); for the even n-th generation use the "side to side"
 *          version; a(n) is the number of triangles a...
 * https://oeis.org/A261956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-8) - a(n-10) for n > 13, with initial terms:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=9, a(4)=12, a(5)=18, a(6)=15,
 *            a(7)=21, a(8)=21, a(9)=36, a(10)=39, a(11)=54, a(12)=36,
 *            a(13)=54
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static const seq_t init[14] = {
        1, 3, 6, 9, 12, 18, 15, 21, 21, 36, 39, 54, 36, 54
    };

    /* Grow cache if needed */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) {
            abort(); /* allocation failure */
        }
        cache = tmp;
        for (idx_t i = cap; i < new_cap; ++i) {
            cache[i] = 0; /* sentinel */
        }
        cap = new_cap;
    }

    /* Compute all required entries up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (cache[i] != 0) continue; /* already set */
        if (i <= 13) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-2] + cache[i-8] - cache[i-10];
        }
    }

    return cache[n];
}
