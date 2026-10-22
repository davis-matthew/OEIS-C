/*
 * A370349  a(n) is the number of integer triples (x,y,z) satisfying a system
 *          of linear inequalities and congruences specified in the comments.
 * https://oeis.org/A370349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((4*n^3 + 18*n^2 + 24*n + 10)/9)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast to seq_t */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    seq_t val = 4 * n3 + 18 * n2 + 24 * sn + 10;   /* numerator */
    return val / (seq_t)9;        /* integer division */
}
