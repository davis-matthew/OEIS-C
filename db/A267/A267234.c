/*
 * A267234  Number of length-5 0..n arrays with no following elements greater
 *          than or equal to the first repeated value.
 * https://oeis.org/A267234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*n^5 + 37*n^4 + 30*n^3 + 5*n^2) / 12 (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 79, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;
    seq_t n4 = n3 * tn;
    seq_t n5 = n4 * tn;
    seq_t numer = (seq_t)12 * n5 + (seq_t)37 * n4 + (seq_t)30 * n3 + (seq_t)5 * n2;
    return numer / (seq_t)12;
}
