/*
 * A269232  a(n) = (n + 1)*(6*n^2 + 15*n + 4)/2.
 * https://oeis.org/A269232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(6*n^2 + 15*n + 4)/2
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t temp = 6*nn*nn + 15*nn + 4;
    seq_t result = (nn + 1) * temp / 2;
    return result;
}
