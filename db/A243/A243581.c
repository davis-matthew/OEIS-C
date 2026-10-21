/*
 * A243581  Integers of the form 8k + 7 that can be written as a sum of four
 *          distinct squares of the form m, m + 2, m + 3, m + 4, where m == 2
 *          (mod 4).
 * https://oeis.org/A243581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^2 + 40*n + 15
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 8191, u64 n <= 9894,
 *            u128 n <= 9894, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 64 * nn * nn + 40 * nn + 15;
}
