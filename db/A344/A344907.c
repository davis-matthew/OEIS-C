/*
 * A344907  Number of polygon edges formed when every pair of vertices of a
 *          regular (2n-1)-gon is joined by an infinite line.
 * https://oeis.org/A344907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^4 - 22*n^3 + 44*n^2 - 35*n + 9
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 182, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast idx_t to seq_t before each multiplication to avoid overflow
     * of the original type */
    seq_t n1 = (seq_t)n;          /* n */
    seq_t n2 = n1 * n1;           /* n^2 */
    seq_t n3 = n2 * n1;           /* n^3 */
    seq_t n4 = n2 * n2;           /* n^4 */

    /* compute the polynomial */
    seq_t res = 4 * n4;           /* 4*n^4 */
    res -= 22 * n3;               /* -22*n^3 */
    res += 44 * n2;               /* +44*n^2 */
    res -= 35 * n1;               /* -35*n  */
    res += 9;                     /* +9     */

    return res;
}
