/*
 * A339267  Level of the Calkin-Wilf tree in which the n-th convergent of the
 *          continued fraction for e appears.
 * https://oeis.org/A339267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3)^2 + n + 1
 * Range:     u8 n <= 44, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)(n / 3);   /* floor(n/3) */
    return m * m + (seq_t)n + ((seq_t)1);
}
