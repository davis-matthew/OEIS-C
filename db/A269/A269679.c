/*
 * A269679  Number of length-4 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus or minus one
 *          modulo n+1.
 * https://oeis.org/A269679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + 3*n^2 + 2*n + 2 for n > 1, a(1) = 10
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)10;
    }
    seq_t nn = (seq_t)n;
    return nn * nn * nn * nn
           + (seq_t)4 * nn * nn * nn
           + (seq_t)3 * nn * nn
           + (seq_t)2 * nn
           + (seq_t)2;
}
