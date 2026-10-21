/*
 * A270994  a(n) = 9454129 + 11184810*n.
 * https://oeis.org/A270994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9454129 + 11184810*n
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)11184810 * (seq_t)n) + (seq_t)9454129;
}
