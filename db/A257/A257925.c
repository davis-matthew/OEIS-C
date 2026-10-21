/*
 * A257925  a(n) = (n^2 - n + 1)*(n^2 + n - 1).
 * https://oeis.org/A257925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n + 1)*(n^2 + n - 1)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n * (seq_t)n;      // n^2
    seq_t term1 = nn - (seq_t)n + 1;       // n^2 - n + 1
    seq_t term2 = nn + (seq_t)n - 1;       // n^2 + n - 1
    return term1 * term2;
}
