/*
 * A398517  a(n) = (4*n^3-3*n^2+5*n+6)/6.
 * https://oeis.org/A398517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3-3*n^2+5*n+6)/6
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;                 /* convert index to seq_t */
    seq_t term = 4 * N * N * N;   /* 4*n^3 */
    term -= 3 * N * N;            /* -3*n^2 */
    term += 5 * N;                /* +5*n   */
    term += 6;                    /* +6     */
    return term / 6;              /* divide by 6 (exact) */
}
