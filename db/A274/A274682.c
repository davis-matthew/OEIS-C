/*
 * A274682  Numbers n such that 8*n-1 is a triangular number.
 * https://oeis.org/A274682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^2 - 11*n + 4)/2 if n is even a(n) = (8*n^2 - 5*n +
 *            1)/2 if n is odd These formulas are equivalent to the general
 *            expression a(n) = (5+3*(-1)^n-2*(8+3*(-1)^n)*n+16*n^2)/4
 * Range:     u8 n <= 6, u16 n <= 96, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {           /* n is even */
        seq_t num = 8 * nn * nn - 11 * nn + 4;
        return num / 2;
    } else {                       /* n is odd */
        seq_t num = 8 * nn * nn - 5 * nn + 1;
        return num / 2;
    }
}
