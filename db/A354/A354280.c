/*
 * A354280  a(n) is the numerator of Cesàro means sequence c(n) of A237420
 *          when the denominator is A141310(n).
 * https://oeis.org/A354280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = n*(n+1), a(2*n+1) = n
 * Range:     u8 n <= 31, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even indices */
    if ((n & 1) == 0) {
        idx_t m = n >> 1;            /* n = 2*m */
        return (seq_t)m * ((seq_t)m + 1);   /* m*(m+1) */
    }
    /* Odd indices */
    idx_t m = n >> 1;                /* n = 2*m+1 */
    return (seq_t)m;                 /* m */
}
