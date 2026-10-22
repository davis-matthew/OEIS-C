/*
 * A300968  Number of n X 3 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A300968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) – 2*a(n-2) – 11*a(n-3) – a(n-4) + 10*a(n-5) +
 *            6*a(n-6) – 4*a(n-7) – 4*a(n-8) – 2*a(n-9) for n > 11 Initial
 *            values: a(1)=1, a(2)=11, a(3)=72, a(4)=459, a(5)=2956,
 *            a(6)=18996, a(7)=122043, a(8)=784108, a(9)=5037730,
 *            a(10)=32366408, a(11)=207947716
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of values, index 0 unused */
static idx_t cached_max = 0;    /* largest n that has been computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* offset starts at 1 */

    /* ensure storage at least up to n */
    if (n > cached_max) {
        size_t new_size = (size_t)(n + 1);  /* indices 0..n   */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;

        /* initial values a(1) … a(11) */
        static const seq_t init[12] = {
            (seq_t)0,                       /* dummy for index 0 */
            (seq_t)1,                       /* a(1) */
            (seq_t)11,                      /* a(2) */
            (seq_t)72,                      /* a(3) */
            (seq_t)459,                     /* a(4) */
            (seq_t)2956,                    /* a(5) */
            (seq_t)18996,                   /* a(6) */
            (seq_t)122043,                  /* a(7) */
            (seq_t)784108,                  /* a(8) */
            (seq_t)5037730,                 /* a(9) */
            (seq_t)32366408,                /* a(10) */
            (seq_t)207947716                /* a(11) */
        };

        /* compute missing values up to n */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i <= 11) {
                cache[i] = init[i];
            } else {
                seq_t val = (seq_t)7 * cache[i-1];
                val -= (seq_t)2 * cache[i-2];
                val -= (seq_t)11 * cache[i-3];
                val -= cache[i-4];
                val += (seq_t)10 * cache[i-5];
                val += (seq_t)6 * cache[i-6];
                val -= (seq_t)4 * cache[i-7];
                val -= (seq_t)4 * cache[i-8];
                val -= (seq_t)2 * cache[i-9];
                cache[i] = val;
            }
        }
        cached_max = n;
    }

    return cache[n];
}
