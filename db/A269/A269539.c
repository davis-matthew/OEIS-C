/*
 * A269539  Number of length-5 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than one.
 * https://oeis.org/A269539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 4*n^3 + 6*n^2 - 2*n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t nn4 = nn3 * nn;
    seq_t nn5 = nn4 * nn;

    seq_t result = nn5 + 5 * nn4 + 4 * nn3 + 6 * nn2 - 2 * nn;
    return result;
}
