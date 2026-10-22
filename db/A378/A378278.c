/*
 * A378278  Cogrowth sequence of the 20-element Frobenius group Frob20 = <S,T
 *          | S^5, T^4, (ST)^4>.
 * https://oeis.org/A378278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A378278: Cogrowth sequence of the 20-element Frobenius group Frob20.
   Sequence is defined by the rational generating function

        (x^6 + x^4 + x^3 - 3*x^2 + 3*x - 1) /
        ((2*x-1) * (x+1) * (x^2+1) * (2*x^2-2*x+1)).

   From this function a linear recurrence of order 6 follows:
        a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 2*a(n-4)
               - 2*a(n-5) + 4*a(n-6)   for n >= 7
   with initial values
        a(0) = 1,  a(1) = 0,  a(2) = 0,  a(3) = 0,
        a(4) = 1,  a(5) = 1,  a(6) = 3.

   The function below memoises computed values in a static array so that
   calls in any order are efficient.  It works for any seq_t/idx_t
   definitions supplied by the build environment.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* coefficients needed for the first seven terms
       (derived from the numerator of the generating function)   */
    static const int c[7] = { -1, 3, -3, 1, 1, 0, 1 };
    static const seq_t init[7] = { 1, 0, 0, 0, 1, 1, 3 };

    /* initialise cache with the first seven values */
    if (cache_cap == 0) {
        cache_cap = 7;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
    }

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;    /* allocation failed – caller can handle */
        cache = tmp;

        for (idx_t i = cache_cap; i <= n; ++i) {
            if (i <= 6) {          /* already set by init array */
                cache[i] = init[i];
                continue;
            }

            /* compute a(i) from the recurrence:
               a(i) = 3*a(i-1) - 3*a(i-2) + a(i-3) + 2*a(i-4)
                      - 2*a(i-5) + 4*a(i-6)            */
            seq_t left = 0, right = 0, tmp_mul;

            /* left part: 3*a[i-1] + a[i-3] + 2*a[i-4] + 4*a[i-6] */
            tmp_mul = cache[i - 1];
            tmp_mul = tmp_mul * (seq_t)3;
            left += tmp_mul;

            if (i >= 3) left += cache[i - 3];

            if (i >= 4) {
                tmp_mul = cache[i - 4];
                tmp_mul = tmp_mul * (seq_t)2;
                left += tmp_mul;
            }

            if (i >= 6) {
                tmp_mul = cache[i - 6];
                tmp_mul = tmp_mul * (seq_t)4;
                left += tmp_mul;
            }

            /* right part: 3*a[i-2] + 2*a[i-5] */
            if (i >= 2) {
                tmp_mul = cache[i - 2];
                tmp_mul = tmp_mul * (seq_t)3;
                right += tmp_mul;
            }

            if (i >= 5) {
                tmp_mul = cache[i - 5];
                tmp_mul = tmp_mul * (seq_t)2;
                right += tmp_mul;
            }

            /* a[i] = left - right  (always non‑negative) */
            cache[i] = left - right;
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
