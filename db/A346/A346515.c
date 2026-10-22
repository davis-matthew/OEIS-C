/*
 * A346515  a(n) = n*(n+2)*(n+4)*(n+6)*(n+8)*(n+10)*(n+12)*(n+14).
 * https://oeis.org/A346515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)*(n+4)*(n+6)*(n+8)*(n+10)*(n+12)*(n+14)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 9, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = n, end = n + 14; i <= end; i += 2)
        res *= (seq_t)i;
    return res;
}
