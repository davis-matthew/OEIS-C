/*
 * A399341  a(n) = (n^6 + 9*n^5 + 30*n^4 + 45*n^3 + 29*n^2 + 6*n) / 8. (Row 3
 *          of array A399342.)
 * https://oeis.org/A399341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 9*n^5 + 30*n^4 + 45*n^3 + 29*n^2 + 6*n) / 8
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 55, u64 n <= 2297,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the required type and use a 128‑bit intermediate
       to avoid overflow of the intermediate numerator. */
    seq_t      n_t = (seq_t)n;
    unsigned __int128 UN = (unsigned __int128)n_t;

    unsigned __int128 n2 = UN * UN;          /* n^2   */
    unsigned __int128 n3 = n2 * UN;          /* n^3   */
    unsigned __int128 n4 = n3 * UN;          /* n^4   */
    unsigned __int128 n5 = n4 * UN;          /* n^5   */
    unsigned __int128 n6 = n5 * UN;          /* n^6   */

    unsigned __int128 num = n6
                         + (unsigned __int128)9 * n5
                         + (unsigned __int128)30 * n4
                         + (unsigned __int128)45 * n3
                         + (unsigned __int128)29 * n2
                         + (unsigned __int128)6 * UN;

    /* divide by 8 (i.e., shift right by 3 bits) */
    num >>= 3;

    return (seq_t)num;
}
