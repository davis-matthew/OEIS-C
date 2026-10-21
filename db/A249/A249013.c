/*
 * A249013  a(n) = floor( (n-1) * (n+4) / 10 ).
 * https://oeis.org/A249013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*(n+4)/10)
 * Range:     u8 n <= 14, u16 n <= 316, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    seq_t nn = (seq_t)n;
    seq_t t1 = nn - (seq_t)1;   /* n-1 */
    seq_t t2 = nn + (seq_t)4;   /* n+4 */
    seq_t prod = t1 * t2;       /* (n-1)*(n+4) */
    return prod / (seq_t)10;    /* floor division by 10 */
}
