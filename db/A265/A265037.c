/*
 * A265037  G.f.: (1 + 22*x - 34*x^2 + 14*x^3)/((1 - x)^2*(1 - 6*x + 8*x^2)).
 * https://oeis.org/A265037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) − 21*a(n-2) + 22*a(n-3) − 8*a(n-4) initial
 *            values: a(0) = 1, a(1) = 30, a(2) = 185, a(3) = 886
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    switch (n) {
        case 0: return 1;
        case 1: return 30;
        case 2: return 185;
        case 3: return 886;
        default: break;
    }

    /* Variables to hold the last four computed values */
    seq_t a0 = 1;     /* a(n-4) */
    seq_t a1 = 30;    /* a(n-3) */
    seq_t a2 = 185;   /* a(n-2) */
    seq_t a3 = 886;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = 8 * a3;        /* 8*a(n-1) */
        an -= 21 * a2;            /* -21*a(n-2) */
        an += 22 * a1;            /* +22*a(n-3) */
        an -= 8 * a0;             /* -8*a(n-4) */

        /* Shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;   /* a(n) */
}
