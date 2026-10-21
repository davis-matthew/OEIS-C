/*
 * A299593  Number of n X 6 0..1 arrays with every element unequal to 0, 1 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A299593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) - a(n-5) - a(n-6) for
 *            n>9 with initial values
 *            a(1)=13,a(2)=15,a(3)=35,a(4)=121,a(5)=256,a(6)=576,a(7)=1225,a(8)=2601,a(9)=5625
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 26, u64 n <= 55, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return 0; /* safety: undefined for n==0 */

    if (n > cache_size) {
        idx_t new_size = n;
        /* reallocate cache to hold indices 1..new_size */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (size_t)(new_size + 1));
        if (!tmp) abort();      /* out of memory */
        cache = tmp;

        /* initial values for n = 1..9 */
        static const seq_t base_vals[10] = {0, 13, 15, 35, 121, 256, 576, 1225, 2601, 5625};

        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            if (i <= 9) {
                cache[i] = base_vals[i];
            } else {
                seq_t t = cache[i-1] + cache[i-2] + cache[i-4] + cache[i-3] * 3;
                t -= cache[i-5];
                t -= cache[i-6];
                cache[i] = t;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
