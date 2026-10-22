/*
 * A355509  Peaceable coexisting armies of knights: a(n) is the maximum number
 *          m such that m white knights and m black knights can coexist on an
 *          n X n chessboard without attacking each other.
 * https://oeis.org/A355509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((n-1)^2)/2) for n >= 7; special values: a(1)=0,
 *            a(2)=2, a(3)=3, a(4)=6, a(5)=10, a(6)=14
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)6;
    if (n == 5) return (seq_t)10;
    if (n == 6) return (seq_t)14;

    seq_t t = (seq_t)n - 1;
    return (t * t) / 2;
}
