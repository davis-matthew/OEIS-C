/*
 * A381424  Truncated hex numbers: a(n) = 24*n^2 + 6*n + 1.
 * https://oeis.org/A381424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*n^2 + 6*n + 1
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return tn * (tn * 24 + 6) + 1;
}
