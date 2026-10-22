/*
 * A317728  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 5, 6, 7 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A317728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 11*a(n-2) + 11*a(n-3) - 2*a(n-4) - a(n-5) -
 *            2*a(n-6) Initial values: a(1)=1, a(2)=25, a(3)=161, a(4)=1250,
 *            a(5)=9541, a(6)=72715
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache keeps already computed values; 0-based array holds a(1) at cache[0] */
    static seq_t *cache = NULL;
    static idx_t cached = 0;               /* number of elements stored in cache */

    if (n == 0)          /* only used for safety; OEIS offset starts at 1 */
        return (seq_t)0;

    /* Ensure the cache is large enough */
    idx_t needed = (n > 6 ? n : 6);
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();
        cached = needed;
    } else if (needed > cached) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cached = needed;
    }

    /* Initialise the first six values (they are the same every call) */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)25;
    cache[2] = (seq_t)161;
    cache[3] = (seq_t)1250;
    cache[4] = (seq_t)9541;
    cache[5] = (seq_t)72715;

    /* Compute values up to n (recurrence needs a(1)..a(6) already present) */
    for (idx_t k = 7; k <= n; ++k) {
        /* cache indices are zero based: a(k) -> cache[k-1] */
        cache[k-1] =
              6 * cache[k-2]
            + 11 * cache[k-3]
            + 11 * cache[k-4]
            -  2 * cache[k-5]
            -   1 * cache[k-6]
            -  2 * cache[k-7];
    }

    return cache[n-1];
}
