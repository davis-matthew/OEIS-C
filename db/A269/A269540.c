/*
 * A269540  Number of length-6 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than one.
 * https://oeis.org/A269540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 5*n^4 + 12*n^3 - 6*n^2 + 6*n - 2, offset 1
 *            (a(1) = 22)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t n5 = n4 * N;
    seq_t n6 = n5 * N;

    /* Positive terms of the formula */
    seq_t pos = n6
              + ((seq_t)6 * n5)
              + ((seq_t)5 * n4)
              + ((seq_t)12 * n3)
              + ((seq_t)6 * N);

    /* Negative terms (including the constant -2) */
    seq_t neg = ((seq_t)6 * n2) + 2;

    return pos - neg;
}
