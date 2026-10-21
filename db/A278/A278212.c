/*
 * A278212  Maximum total number of possible moves that any number of bishops
 *          of the same color can make on an n X n chessboard.
 * https://oeis.org/A278212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(n-2)^2 for n >= 6; a(1) = 0, a(2) = 2, a(3) = 8, a(4)
 *            = 20, a(5) = 38
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)2;
    if (n == 3)
        return (seq_t)8;
    if (n == 4)
        return (seq_t)20;
    if (n == 5)
        return (seq_t)38;

    /* For n >= 6 */
    seq_t m = (seq_t)(n - 2);
    return (seq_t)4 * m * m;
}
