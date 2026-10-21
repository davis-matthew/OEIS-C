/*
 * A299670  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -3*a(n-2) -12*a(n-4) -15*a(n-5) +20*a(n-6)
 *            +17*a(n-7) +32*a(n-8) -64*a(n-9) -5*a(n-10) +25*a(n-11)
 *            +77*a(n-12) +11*a(n-13) -27*a(n-14) -21*a(n-15) -14*a(n-16)
 *            -13*a(n-17) -4*a(n-18) for n > 19, with initial values
 *            a(1)..a(19)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of elements stored (a(1) at cache[0]) */

    /* Initial values a(1) .. a(19) (0-indexed in cache) */
    static const seq_t init[19] = {
        (seq_t)4ULL,
        (seq_t)26ULL,
        (seq_t)94ULL,
        (seq_t)372ULL,
        (seq_t)1512ULL,
        (seq_t)6133ULL,
        (seq_t)24742ULL,
        (seq_t)100035ULL,
        (seq_t)404608ULL,
        (seq_t)1636222ULL,
        (seq_t)6616385ULL,
        (seq_t)26756052ULL,
        (seq_t)108199516ULL,
        (seq_t)437548895ULL,
        (seq_t)1769407046ULL,
        (seq_t)7155326538ULL,
        (seq_t)28935512224ULL,
        (seq_t)117012660592ULL,
        (seq_t)473188897749ULL
    };

    if (n == 0) /* undefined; return 0 */
        return (seq_t)0;

    /* Grow cache if needed */
    if (n > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache)
            abort();              /* out of memory */
        cache = new_cache;

        for (idx_t i = cache_cap; i < n; ++i) {
            if (i < 19) {
                cache[i] = init[i];
            } else {
                seq_t sum = (seq_t)0;
                sum += cache[i-1] * (seq_t)5;
                sum -= cache[i-2] * (seq_t)3;
                sum -= cache[i-4] * (seq_t)12;
                sum -= cache[i-5] * (seq_t)15;
                sum += cache[i-6] * (seq_t)20;
                sum += cache[i-7] * (seq_t)17;
                sum += cache[i-8] * (seq_t)32;
                sum -= cache[i-9] * (seq_t)64;
                sum -= cache[i-10] * (seq_t)5;
                sum += cache[i-11] * (seq_t)25;
                sum += cache[i-12] * (seq_t)77;
                sum += cache[i-13] * (seq_t)11;
                sum -= cache[i-14] * (seq_t)27;
                sum -= cache[i-15] * (seq_t)21;
                sum -= cache[i-16] * (seq_t)14;
                sum -= cache[i-17] * (seq_t)13;
                sum -= cache[i-18] * (seq_t)4;
                cache[i] = sum;   /* truncation occurs automatically */
            }
        }
        cache_cap = n;
    }

    return cache[n-1];
}
