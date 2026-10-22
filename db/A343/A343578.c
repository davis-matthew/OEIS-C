/*
 * A343578  a(n) = 32*n^2 - 40*n + 10.
 * https://oeis.org/A343578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^2 - 40*n + 10
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t nn = sn * sn;          /* n^2 */
    seq_t res = nn * 32;         /* 32*n^2 */
    res -= sn * 40;              /* -40*n */
    res += 10;                   /* +10 */
    return res;
}
