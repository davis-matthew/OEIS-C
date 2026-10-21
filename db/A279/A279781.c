/*
 * A279781  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = 9/5.
 * https://oeis.org/A279781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = -2 *
 *            (a(n-1)+a(n-2)+a(n-3)+a(n-4)) for n >= 7 with initial terms
 *            a(0)=1, a(1)=-3, a(2)=4, a(3)=-4, a(4)=4, a(5)=-3, a(6)=-1. This
 *            follows from the generating function G(x) =
 *            (1-x)(1-x^5)/(1+2x+2x^2+2x^3+2x^4)
 * Range:     i8 n <= 15, i16 n <= 34, i32 n <= 74, i64 n <= 149,
 *            i128 n <= 303, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values a(0) .. a(6) */
    static const seq_t init[7] = { (seq_t)1, (seq_t)-3, (seq_t)4, (seq_t)-4,
                                   (seq_t)4, (seq_t)-3, (seq_t)-1 };

    if (n <= 6)
        return init[n];

    /* set up the last four values for n = 7 */
    seq_t prev4 = init[3];   /* a(3) */
    seq_t prev3 = init[4];   /* a(4) */
    seq_t prev2 = init[5];   /* a(5) */
    seq_t prev1 = init[6];   /* a(6) */
    seq_t cur = 0;

    for (idx_t i = 7; i <= n; ++i) {
        cur = -((seq_t)2 * (prev1 + prev2 + prev3 + prev4));
        /* shift the window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
