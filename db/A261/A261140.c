/*
 * A261140  a(n) = 3486107472997423 + (n-1)*371891575525470.
 * https://oeis.org/A261140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3486107472997423 + (n-1)*371891575525470
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)3486107472997423ull + ((seq_t)(n - 1)) * (seq_t)371891575525470ull;
}
