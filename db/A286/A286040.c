/*
 * A286040  Matching number of the n X n antelope graph.
 * https://oeis.org/A286040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the recurrence * a(n) = 2*a(n-1) - 2*a(n-3) +
 *            a(n-4) + b(n), * with initial values a(1)=a(2)=a(3)=a(4)=0, *
 *            where b(n) is the coefficient of x^n in the numerator of the *
 *            generating function * x^5(4 + 4x - 12x^3 - 3x^4 + 10x^5 + x^6 +
 *            4x^8 + 2x^9 * -13x^10 - 6x^11 + 7x^12 + 10x^13...
 * Range:     u8 n <= 22, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int64_t b_coeff[24] = {
    0,  /* 0 */
    0,  /* 1 */
    0,  /* 2 */
    0,  /* 3 */
    0,  /* 4 */
    4,  /* 5 */
    4,  /* 6 */
    0,  /* 7 */
   -12, /* 8 */
    -3, /* 9 */
    10, /*10 */
     1, /*11 */
     0, /*12 */
     4, /*13 */
     2, /*14 */
    -13,/*15 */
    -6, /*16 */
     7, /*17 */
    10, /*18 */
     0, /*19 */
    -4, /*20 */
    -4, /*21 */
    -2, /*22 */
     4  /*23 */
};

seq_t a(idx_t n)
{
    if (n <= 4) {
        return 0;
    }

    /* maintain the last four values a_{i-4}, a_{i-3}, a_{i-2}, a_{i-1} */
    seq_t a_m4 = 0,    /* a(i-4) */
          a_m3 = 0,    /* a(i-3) */
          a_m2 = 0,    /* a(i-2) */
          a_m1 = 0;    /* a(i-1) */
    seq_t a_i = 0;

    for (idx_t i = 5; i <= n; ++i) {
        long long term = 2LL * (long long)a_m1
                       - 2LL * (long long)a_m3
                       + (long long)a_m4;

        if (i <= 23) {
            term += b_coeff[i];
        }

        a_i = (seq_t)term;

        /* shift window for next iteration */
        a_m4 = a_m3;
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = a_i;
    }

    return a_i;
}
