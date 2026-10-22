/*
 * A342939  a(n) is the Skolem number of the triangular grid graph T_n.
 * https://oeis.org/A342939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*(n-1)/2 (exception a(3)=5)
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined below the offset */
    if (n == 3)
        return 5;            /* special case */
    seq_t nn = (seq_t)n;    /* cast index to seq_t for safe arithmetic */
    return 1 + nn * (nn - 1) / 2;
}
