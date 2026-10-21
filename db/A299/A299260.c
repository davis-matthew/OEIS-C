/*
 * A299260  Partial sums of A299254.
 * https://oeis.org/A299260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/5)*(8*n^3 + 12*n^2 + 14*n + 5 + [n==1(mod5)] -
 *            [n==3(mod5)])
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 976, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t term = (seq_t)8 * n3 + (seq_t)12 * n2 + (seq_t)14 * n_seq + (seq_t)5;

    if ((n % 5) == 1) term += (seq_t)1;
    if ((n % 5) == 3) term -= (seq_t)1;

    return term / (seq_t)5;
}
