/*
 * A264444  a(n) = n*(n + 7)*(n + 14)/6.
 * https://oeis.org/A264444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+7)*(n+14)/6
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    return ns * (ns + (seq_t)7) * (ns + (seq_t)14) / (seq_t)6;
}
