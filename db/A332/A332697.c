/*
 * A332697  a(n) = (n^4 + 5*n^3 + 11*n^2 + 7*n)/6.
 * https://oeis.org/A332697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 5*n^3 + 11*n^2 + 7*n)/6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 259, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the element type early to avoid mixing types */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2  */
    seq_t n3 = n2 * sn;          /* n^3  */
    seq_t n4 = n3 * sn;          /* n^4  */
    /* Combine according to the formula */
    seq_t term = n4 + 5 * n3 + 11 * n2 + 7 * sn;
    return term / 6;
}
