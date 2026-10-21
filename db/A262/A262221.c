/*
 * A262221  a(n) = 25*n*(n + 1)/2 + 1.
 * https://oeis.org/A262221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n*(n+1)/2 + 1
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn + 1) / 2;
    val = val * 25 + 1;
    return val;
}
