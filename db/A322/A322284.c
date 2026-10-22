/*
 * A322284  Number of nonequivalent ways to place n nonattacking kings on a 2
 *          X 2n chessboard under all symmetry operations of the rectangle.
 * https://oeis.org/A322284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*2^(n-2) + (1+(-1)^n)^(n/2-1) for n>1, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    seq_t term1 = (seq_t)(n + 1) * pow2(n - 2);
    seq_t term2 = 0;
    if ((n & 1) == 0) {            /* n is even */
        term2 = pow2(n / 2 - 1);
    }
    return term1 + term2;
}
