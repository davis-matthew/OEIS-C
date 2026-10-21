/*
 * A241765  a(n) = n*(n + 1)*(n + 2)*(3*n + 17)/24.
 * https://oeis.org/A241765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(3*n+17)/24
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 231, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = nn * (nn + 1) * (nn + 2) * (3 * nn + 17);
    term /= 24;
    return term;
}
