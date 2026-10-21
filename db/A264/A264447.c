/*
 * A264447  a(n) = n*(n + 7)*(n + 14)*(n + 21)/24.
 * https://oeis.org/A264447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+7)*(n+14)*(n+21)/24
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                         /* cast index to seq_t */
    return nn * (nn + 7) * (nn + 14) * (nn + 21) / 24;
}
