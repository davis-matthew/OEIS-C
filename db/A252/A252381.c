/*
 * A252381  Number of (n+2) X (5+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 3 5 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 7.
 * https://oeis.org/A252381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-3) - 4*a(n-6) + a(n-9) for n > 12 Initial values:
 *            a(1)=3020, a(2)=1067, a(3)=963, a(4)=1091, a(5)=1305, a(6)=1667,
 *            a(7)=2031, a(8)=2598, a(9)=3526, a(10)=4492, a(11)=5983,
 *            a(12)=8393
 * Range:     u8 none, u16 n <= 18, u32 n <= 53, u64 n <= 122, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;     /* allocated size */
    static idx_t computed = 0;        /* highest n computed so far */

    /* pre‑defined initial terms (index 1..12) */
    static const seq_t init[13] = {
        0,          /* unused index 0 */
        3020, 1067, 963, 1091, 1305,
        1667, 2031, 2598, 3526, 4492,
        5983, 8393
    };

    if (n == 0) return 0;            /* safety for illegal n */

    /* Allocate / reallocate cache large enough for n */
    size_t needed = (size_t)n + 1;      /* indices 0..n inclusive */
    if (needed > cache_size) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cache_size = needed;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[i] = init[i];
        } else {
            /* recurrence: a(i) = 4*a(i-3) - 4*a(i-6) + a(i-9) */
            seq_t t1 = cache[i-3];
            seq_t t2 = cache[i-6];
            seq_t t3 = cache[i-9];
            cache[i] = ( (seq_t)4 * t1 ) - ( (seq_t)4 * t2 ) + t3;
        }
    }
    computed = n;

    return cache[n];
}
