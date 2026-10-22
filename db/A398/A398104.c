/*
 * A398104  Number of minimal dominating sets in the 3 X n king graph.
 * https://oeis.org/A398104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence derived from the generating function:
   (1 + 2*x + 3*x^2 + 3*x^3 - 21*x^5 - 6*x^6 - 18*x^8 - 5*x^9 - 2*x^10
   - 3*x^11 - 2*x^13)/(1 - 3*x^2 - 3*x^3 - 12*x^4 - 7*x^5 + 20*x^6
   - 2*x^8 + 15*x^9 + 4*x^10 + 3*x^11 + 4*x^12 + 2*x^13 + 2*x^14)
   yields the linear recurrence
   a(n) = 3 a(n-2) + 3 a(n-3) + 12 a(n-4) + 7 a(n-5) - 20 a(n-6)
          + 2 a(n-8) - 15 a(n-9) - 4 a(n-10) - 3 a(n-11) - 4 a(n-12)
          - 2 a(n-13) - 2 a(n-14)   for n ≥ 14,
   with the initial values a(0)…a(13) given by the numerator coefficients.
*/

#include <stdlib.h>   /* for malloc, realloc, abort */

static const int16_t p[14] = {
    1,    /* p(0) */
    2,    /* p(1) */
    3,    /* p(2) */
    3,    /* p(3) */
    0,    /* p(4) */
   -21,   /* p(5) */
   -6,    /* p(6) */
    0,    /* p(7) */
   -18,   /* p(8) */
   -5,    /* p(9) */
   -2,    /* p(10) */
   -3,    /* p(11) */
    0,    /* p(12) */
   -2     /* p(13) */
};

static const int16_t q[15] = {
    1,    /* q(0) */
    0,    /* q(1) */
   -3,    /* q(2) */
   -3,    /* q(3) */
   -12,   /* q(4) */
   -7,    /* q(5) */
   20,    /* q(6) */
    0,    /* q(7) */
   -2,    /* q(8) */
   15,    /* q(9) */
    4,    /* q(10) */
    3,    /* q(11) */
    4,    /* q(12) */
    2,    /* q(13) */
    2     /* q(14) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t pos = 0, sub = 0;
            /* contributions from the denominator */
            for (idx_t k = 1; k <= 14; ++k) {
                if (i >= k) {
                    int16_t ck = q[k];
                    if (ck < 0)
                        pos += cache[i - k] * (seq_t)(-ck);
                    else if (ck > 0)
                        sub += cache[i - k] * (seq_t)ck;
                }
            }
            /* contribution from the numerator */
            int16_t pi = (i < 14) ? p[i] : 0;
            if (pi < 0)
                sub += (seq_t)(-pi);
            else
                pos += (seq_t)pi;

            cache[i] = pos - sub;
        }
        cache_len = new_len;
    }
    return cache[n];
}
