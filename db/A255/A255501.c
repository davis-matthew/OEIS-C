/*
 * A255501  a(n) = (n^9 + 5*n^8 + 4*n^7 - n^6 - 5*n^5 + 2*n^4)/6.
 * https://oeis.org/A255501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^9 + 5*n^8 + 4*n^7 - n^6 - 5*n^5 + 2*n^4)/6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 168, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 m = n;
    unsigned __int128 n2 = m * m;          /* n^2 */
    unsigned __int128 n3 = n2 * m;         /* n^3 */
    unsigned __int128 n4 = n3 * m;         /* n^4 */
    unsigned __int128 n5 = n4 * m;         /* n^5 */
    unsigned __int128 n6 = n5 * m;         /* n^6 */
    unsigned __int128 n7 = n6 * m;         /* n^7 */
    unsigned __int128 n8 = n7 * m;         /* n^8 */
    unsigned __int128 n9 = n8 * m;         /* n^9 */

    unsigned __int128 numerator =
          n9 + 5 * n8 + 4 * n7 - n6 - 5 * n5 + 2 * n4;

    unsigned __int128 result = numerator / 6;
    return (seq_t)result;
}
