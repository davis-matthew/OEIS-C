/*
 * A285766  Maximum spillway height for a zero or one bend minimal area lake
 *          in a number square.
 * https://oeis.org/A285766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0, a(2)=6; a(n) = n^2 - 2*n + 7 for n>2
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        return 0;
    }
    if (n == 2) {
        return 6;
    }
    seq_t nn = (seq_t)n;
    return nn * nn - (seq_t)2 * nn + (seq_t)7;
}
