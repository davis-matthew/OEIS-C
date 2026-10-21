/*
 * A259060  Numbers that are representable in at least two ways as sums of
 *          four distinct nonvanishing cubes.
 * https://oeis.org/A259060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n+9)*(2*n^2 + 36*n + 357)
 * Range:     u8 none, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          /* convert index to seq_t */
    seq_t term1 = nn + (seq_t)9;    /* (n + 9) */
    seq_t term2 = (seq_t)2*nn*nn + (seq_t)36*nn + (seq_t)357; /* 2*n^2 + 36*n + 357 */
    return (seq_t)2*term1*term2;    /* 2*(n + 9)*(2*n^2 + 36*n + 357) */
}
