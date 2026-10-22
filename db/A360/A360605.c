/*
 * A360605  The polygonal polynomials evaluated at x = -1/2 and normalized
 *          with (-2)^n.
 * https://oeis.org/A360605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = - a(n-1) + 5*a(n-2) + a(n-3) - 8*a(n-4) + 4*a(n-5);
 *            initial values: a(0)=0, a(1)=1, a(2)=0, a(3)=1, a(4)=0
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 30, i64 n <= 60, i128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)0;

    seq_t a0 = 0, a1 = 1, a2 = 0, a3 = 1, a4 = 0;
    seq_t ai;
    for (idx_t i = 5; i <= n; ++i) {
        ai = -a4 + 5 * a3 + a2 - 8 * a1 + 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}
