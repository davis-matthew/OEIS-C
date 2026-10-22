/*
 * A300758  a(n) = 2n*(n+1)*(2n+1).
 * https://oeis.org/A300758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n+1)*(2*n+1)
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = nn * (nn + 1);
    term *= (nn * (seq_t)2 + 1);
    term *= (seq_t)2;
    return term;
}
