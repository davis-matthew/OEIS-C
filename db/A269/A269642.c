/*
 * A269642  Number of length-5 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus two or minus
 *          1.
 * https://oeis.org/A269642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 4*n^3 + 6*n^2 - 5*n + 1 (empirical)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n;
    seq_t n2 = m * m;
    seq_t n3 = n2 * m;
    seq_t n4 = n3 * m;
    seq_t n5 = n4 * m;

    seq_t ans = n5
                + (seq_t)5 * n4
                + (seq_t)4 * n3
                + (seq_t)6 * n2
                - (seq_t)5 * m
                + (seq_t)1;
    return ans;
}
