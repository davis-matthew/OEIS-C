/*
 * A253285  a(n) = RF(n+1,3)*C(n+2,n-1), where RF(a,n) is the rising
 *          factorial.
 * https://oeis.org/A253285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * ((n+1)*(n+2))^2 * (n+3) / 6
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* compute (n+1)*(n+2) */
    seq_t tmp = (N + 1) * (N + 2);
    /* square it */
    seq_t tmp2 = tmp * tmp;
    /* multiply by n*(n+3) and divide by 6 */
    seq_t result = N * tmp2 * (N + 3) / 6;
    return result;
}
