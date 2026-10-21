/*
 * A261807  a(n) = n XOR n^3.
 * https://oeis.org/A261807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n XOR n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n3 = nn * nn * nn;
    return nn ^ n3;
}
