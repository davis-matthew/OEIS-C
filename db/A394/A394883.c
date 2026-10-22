/*
 * A394883  a(n) = n^2*(n + 1)*(n + 2)*(2*n + 1)*(2*n + 3)*(5*n - 1)/360.
 * https://oeis.org/A394883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n + 1) * (n + 2) * (2*n + 1) * (2*n + 3) * (5*n -
 *            1) / 360
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 35, u64 n <= 853,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the sequence, but safe */

    /* Use an unsigned __int128 temporaries to avoid intermediate overflow. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 prod = nn * nn;           /* n^2 */
    prod *= (nn + 1);                           /* * (n+1) */
    prod *= (nn + 2);                           /* * (n+2) */
    prod *= (2 * nn + 1);                       /* * (2n+1) */
    prod *= (2 * nn + 3);                       /* * (2n+3) */
    prod *= (5 * nn - 1);                       /* * (5n-1) */
    prod /= 360;                                /* divide by 360 */

    return (seq_t)prod;
}
