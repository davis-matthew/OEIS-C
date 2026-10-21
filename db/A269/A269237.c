/*
 * A269237  a(n) = (n + 1)^2*(5*n^2 + 10*n + 2)/2.
 * https://oeis.org/A269237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)^2 * (5*n^2 + 10*n + 2) / 2
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;          /* convert index to seq_t */
    seq_t sn1 = sn + 1;
    seq_t t1  = sn1 * sn1;                    /* (n+1)^2 */
    seq_t t2  = 5 * sn * sn + 10 * sn + 2;    /* 5*n^2 + 10*n + 2 */
    return (t1 * t2) / 2;
}
