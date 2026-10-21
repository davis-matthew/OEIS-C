/*
 * A269585  Number of length-5 0..n arrays with no repeated value differing
 *          from the previous repeated value by more than one.
 * https://oeis.org/A269585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 7*n^3 + 12*n^2 + 6*n + 1 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    seq_t res = (seq_t)1;
    res = res * s + (seq_t)5;
    res = res * s + (seq_t)7;
    res = res * s + (seq_t)12;
    res = res * s + (seq_t)6;
    res = res * s + (seq_t)1;
    return res;
}
