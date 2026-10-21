/*
 * A243578  Integers j of the form 8k+7 that are sum of distinct squares of
 *          the form m, m+1, m+2, m+4, where m == 1 (mod 4).
 * https://oeis.org/A243578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^2 - 40*n + 15
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 8192, u64 n <= 7866,
 *            u128 n <= 7866, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)64 * sn * sn - (seq_t)40 * sn + (seq_t)15;
}
