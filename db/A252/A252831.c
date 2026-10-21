/*
 * A252831  Number of n X 3 nonnegative integer arrays with upper left 0 and
 *          every value within 2 of its king move distance from the upper left
 *          and every value increasing by 0 or 1 with every step right,
 *          diagonally se or down.
 * https://oeis.org/A252831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40*n^2 - 199*n + 283 for n > 3 Initial values: a(1)=4,
 *            a(2)=13, a(3)=44, a(4)=127, a(5)=288, a(6)=529 Recurrence:
 *            a(n)=3*a(n-1)-3*a(n-2)+a(n-3) for n>6
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    if (n == 1) return 4;
    if (n == 2) return 13;
    if (n == 3) return 44;
    if (n == 4) return 127;
    if (n == 5) return 288;
    if (n == 6) return 529;

    seq_t a_i_minus_3 = 127; /* a(4) */
    seq_t a_i_minus_2 = 288; /* a(5) */
    seq_t a_i_minus_1 = 529; /* a(6) */
    seq_t cur = 0;

    for (idx_t i = 7; i <= n; ++i) {
        cur = 3 * a_i_minus_1 - 3 * a_i_minus_2 + a_i_minus_3;
        a_i_minus_3 = a_i_minus_2;
        a_i_minus_2 = a_i_minus_1;
        a_i_minus_1 = cur;
    }

    return a_i_minus_1;
}
