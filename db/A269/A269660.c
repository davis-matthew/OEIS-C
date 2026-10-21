/*
 * A269660  Number of length-7 0..n arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 21*n^5 + 25*n^4 + 5*n^3 + 2*n^2 + 11*n - 8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)1;
    r = r * (seq_t)n + 7;
    r = r * (seq_t)n + 21;
    r = r * (seq_t)n + 25;
    r = r * (seq_t)n + 5;
    r = r * (seq_t)n + 2;
    r = r * (seq_t)n + 11;
    r = r * (seq_t)n - 8;
    return r;
}
