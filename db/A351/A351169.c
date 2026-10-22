/*
 * A351169  a(n) is the minimum number of vertices of degree 4 over all
 *          4-collapsible graphs with n vertices.
 * https://oeis.org/A351169
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(2*n/7) for n>7; special cases: a(5)=5, a(6)=4,
 *            a(7)=3
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)              /* undefined for offset, return 0 */
        return 0;

    if (n == 5)
        return 5;
    if (n == 6)
        return 4;
    if (n == 7)
        return 3;

    /* n >= 8 : ceil(2*n/7) == (2*n + 6)/7 using integer arithmetic */
    seq_t v = (seq_t)2 * n;
    return (v + 6) / 7;
}
