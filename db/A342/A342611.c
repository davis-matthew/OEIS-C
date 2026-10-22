/*
 * A342611  a(0) = 0, a(1) = 1; a(2*n) = 7*a(n), a(2*n+1) = a(n) + a(n+1).
 * https://oeis.org/A342611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n=0; 1 if n=1; a(2n)=7*a(n); a(2n+1)=a(n)+a(n+1)
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if ((n & (idx_t)1) == 0) /* even : 2n */
        return (seq_t)7 * a(n >> 1);
    else                      /* odd : 2n+1 */
        return a(n >> 1) + a((n >> 1) + 1);
}
