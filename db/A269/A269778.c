/*
 * A269778  Number of length-6 0..n arrays with every repeated value unequal
 *          to the previous repeated value plus one mod n+1.
 * https://oeis.org/A269778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 15*n^4 + 14*n^3 + 4*n^2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;
    seq_t N6 = N5 * N;
    return N6 + 6 * N5 + 15 * N4 + 14 * N3 + 4 * N2;
}
