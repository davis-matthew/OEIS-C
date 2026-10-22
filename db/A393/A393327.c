/*
 * A393327  a(n) = (1/2)*n*(n+3)*(n+4).
 * https://oeis.org/A393327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*n*(n+3)*(n+4)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 2044, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t res = tn * (tn + 3);
    res *= (tn + 4);
    return res / 2;
}
