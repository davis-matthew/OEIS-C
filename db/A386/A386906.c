/*
 * A386906  a(n) = (3*n - 2)^2*(3*n - 1)/2.
 * https://oeis.org/A386906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n - 2)^2*(3*n - 1)/2
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;           /* convert index to seq_t first */
    seq_t t  = 3 * tn - 2;         /* t = 3*n - 2 */
    seq_t t2 = t * t;              /* t^2 */
    seq_t res = (t2 * (t + 1)) / 2;/* (t^2)*(t+1)/2, where t+1 = 3*n-1 */
    return res;
}
