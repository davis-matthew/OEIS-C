/*
 * A267236  Number of length-7 0..n arrays with no following elements greater
 *          than or equal to the first repeated value.
 * https://oeis.org/A267236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (60*n^7 + 207*n^6 + 210*n^5 + 70*n^4 - 7*n^2)/60
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;
    seq_t n7 = n6 * nn;

    seq_t term1 = (seq_t)60 * n7;
    seq_t term2 = (seq_t)207 * n6;
    seq_t term3 = (seq_t)210 * n5;
    seq_t term4 = (seq_t)70 * n4;
    seq_t sub    = (seq_t)7 * n2;

    seq_t sum = term1 + term2 + term3 + term4;
    sum -= sub;

    return sum / (seq_t)60;
}
