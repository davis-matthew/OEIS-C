/*
 * A268262  Number of length-(3+1) 0..n arrays with new repeated values
 *          introduced in sequential order starting with zero.
 * https://oeis.org/A268262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + n^3 + 3*n^2 + 2*n + 2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n2 * n2;
    return n4 + n3 + 3 * n2 + 2 * nn + 2;
}
