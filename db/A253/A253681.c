/*
 * A253681  Integer squares c^2 that are equal to the sum of an odd number M
 *          of consecutive cubed integers b^3 + (b+1)^3 + ... + (b+M-1)^3 =
 *          c^2 starting at b(n) (A253679).
 * https://oeis.org/A253681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(n+1)*(2*n+1)*(8*n*(n+1)+1))^2
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 41, u128 n <= 3565,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t2 = tn + 1;                     /* n+1 */
    seq_t c  = 2 * tn * t2;                /* 2*n*(n+1) */
    seq_t t3 = 2 * tn + 1;                 /* 2*n+1 */
    c = c * t3;                            /* multiply by (2n+1) */
    seq_t t4 = 8 * tn * t2 + 1;            /* 8*n*(n+1)+1 */
    c = c * t4;                            /* multiply by the last factor */
    return c * c;                          /* square to get a(n) */
}
