/*
 * A291832  Numbers k such that k^6 is sum of two positive 7th powers.
 * https://oeis.org/A291832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64 * n^7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t pow7 = 1;
    for (int i = 0; i < 7; ++i)
        pow7 *= nn;
    return pow7 * 64;
}
