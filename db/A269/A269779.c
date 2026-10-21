/*
 * A269779  Number of length-7 0..n arrays with every repeated value unequal
 *          to the previous repeated value plus one mod n+1.
 * https://oeis.org/A269779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 21*n^5 + 25*n^4 + 11*n^3 + n^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          // n^2
    seq_t n3 = n2 * nn;          // n^3
    seq_t n4 = n3 * nn;          // n^4
    seq_t n5 = n4 * nn;          // n^5
    seq_t n6 = n5 * nn;          // n^6
    seq_t n7 = n6 * nn;          // n^7

    return n7 + 7*n6 + 21*n5 + 25*n4 + 11*n3 + n2;
}
