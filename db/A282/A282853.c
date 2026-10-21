/*
 * A282853  36-gonal numbers: a(n) = n*(17*n-16).
 * https://oeis.org/A282853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(17*n-16)
 * Range:     u8 n <= 4, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn * (seq_t)17 - (seq_t)16);
}
