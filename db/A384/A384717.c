/*
 * A384717  Sum of floored squared chord lengths from -1 to the 2n-th roots of
 *          unity (upper semicircle, endpoints excluded).
 * https://oeis.org/A384717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + floor(n/3) + floor(2n/3)
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t sn = (seq_t)n;

    seq_t t1 = sn >> 1;                  /* floor(n/2) */

    seq_t q3 = sn / 3;                   /* n/3   */
    seq_t r3 = sn % 3;                   /* n%3   */
    seq_t t2 = q3;                       /* floor(n/3)    */

    /* floor(2n/3) = 2*(n/3) + (n%3==2 ? 1 : 0) */
    seq_t t3 = (q3 << 1) + (r3 == 2 ? (seq_t)1 : (seq_t)0);

    return t1 + t2 + t3;
}
