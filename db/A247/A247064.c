/*
 * A247064  Dynamic Betting Game D(n,5,4).
 * https://oeis.org/A247064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(32*n/31)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 32*n/31 = n + n/31, avoiding large intermediate products */
    return (seq_t)n + ((seq_t)n / 31);
}
