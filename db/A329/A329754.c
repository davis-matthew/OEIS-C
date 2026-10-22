/*
 * A329754  Doubly pentagonal pyramidal numbers.
 * https://oeis.org/A329754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(n^3+n^2+2)*(n+1)^2/16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;          /* n^2   */
    unsigned __int128 n3 = n2 * nn;          /* n^3   */
    unsigned __int128 n4 = n2 * n2;          /* n^4   */
    unsigned __int128 part = n3 + n2 + (unsigned __int128)2; /* n^3+n^2+2 */
    unsigned __int128 n1 = nn + (unsigned __int128)1;
    unsigned __int128 n1sq = n1 * n1;        /* (n+1)^2 */
    unsigned __int128 prod = n4 * part;
    prod *= n1sq;
    prod /= (unsigned __int128)16;          /* divide by 2^4 */
    return (seq_t)prod;
}
