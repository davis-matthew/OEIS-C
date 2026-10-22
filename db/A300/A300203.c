/*
 * A300203  Number of n X 3 0..1 arrays with every element equal to 0, 1, 2,
 *          3, 4, 5, 6 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A300203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=32, a(3)=248, a(4)=1933, a(5)=15070 a(n) = 8*a(n-1)
 *            - 2*a(n-2) + 5*a(n-3) - 13*a(n-4) - 6*a(n-5) for n>5 (Linear
 *            recurrence of order 5)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), ... */
static size_t cache_len = 0;   /* number of terms currently computed */
static size_t cache_cap = 0;   /* allocated capacity */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* offset starts at 1 */

    /* Initialize cache with the first five terms */
    if (cache == NULL) {
        cache_cap = 5;
        cache_len = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)248;
        cache[3] = (seq_t)1933;
        cache[4] = (seq_t)15070;
    }

    /* Grow cache if needed (thread‑safe not required by problem) */
    size_t needed = (size_t)n;
    if (needed > cache_len) {
        if (needed > cache_cap) {
            size_t newcap = cache_cap;
            while (newcap < needed) newcap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
            cache_cap = newcap;
        }
        for (size_t i = cache_len; i < needed; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];

            seq_t temp = 0;
            temp = a1 * (seq_t)8;
            temp = temp - a2 * (seq_t)2;
            temp = temp + a3 * (seq_t)5;
            temp = temp - a4 * (seq_t)13;
            temp = temp - a5 * (seq_t)6;
            cache[i] = temp;
        }
        cache_len = needed;
    }

    return cache[n - 1];
}
