/*
 * A287396  a(n) = (7*(csc(2*Pi/7))^2)^n + (7*(csc(4*Pi/7))^2)^n +
 *          (7*(csc(8*Pi/7))^2)^n.
 * https://oeis.org/A287396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 56*a(n-1) - 784*a(n-2) + 3136*a(n-3), a(0) = 3, a(1) =
 *            56, a(2) = 1568
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* number of elements needed to reach index n */
    size_t need = (size_t)n + 1;

    if (!cache || need > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                    /* allocation failure */
        cache = tmp;

        /* initialise the first three terms if they are not yet stored */
        if (need > 0) cache[0] = (seq_t)3;
        if (need > 1) cache[1] = (seq_t)56;
        if (need > 2) cache[2] = (seq_t)1568;

        /* the recurrence only holds for n >= 3 */
        size_t start = cache_len;
        if (start < 3) start = 3;

        /* compute missing values up to the requested index */
        if (start < need) {
            for (size_t i = start; i < need; ++i) {
                seq_t t1 = (seq_t)56 * cache[i - 1];
                seq_t t2 = (seq_t)784 * cache[i - 2];
                seq_t t3 = (seq_t)3136 * cache[i - 3];
                cache[i] = t1 - t2 + t3;
            }
        }

        cache_len = need;
    }

    return cache[(size_t)n];
}
