/*
 * A244876  Number of magic labelings with magic sum n of 8th graph shown in
 *          link.
 * https://oeis.org/A244876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (72*n^7 + 756*n^6 + 3624*n^5 + 10170*n^4 + 18008*n^3 +
 *            20124*n^2 + 13036*n + 3705 + (-1)^n*(60*n^2 + 180*n + 135)) /
 *            3840
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a signed 128‑bit type for all intermediate arithmetic */
    __int128 nn = n;                /* make a 128‑bit copy of n           */
    __int128 n2  = nn * nn;
    __int128 n3  = n2 * nn;
    __int128 n4  = n3 * nn;
    __int128 n5  = n4 * nn;
    __int128 n6  = n5 * nn;
    __int128 n7  = n6 * nn;

    /* (-1)^n: +1 for even n, -1 for odd n */
    __int128 s = (n & 1ULL) ? -1 : 1;

    // numerator of the formula with common denominator 3840
    __int128 num = 0;
    num += (__int128)72   * n7;                     /* 3*n^7/160   */
    num += (__int128)756  * n6;                     /* 63*n^6/320  */
    num += (__int128)3624 * n5;                     /* 151*n^5/160 */
    num += (__int128)10170 * n4;                    /* 339*n^4/128 */
    num += (__int128)18008 * n3;                    /* 2251*n^3/480*/
    num += (__int128)20124 * n2;                    /* 1677*n^2/320*/
    num += (__int128)13036 * nn;                    /* 3259*n/960  */
    num += (__int128)3705;                          /* 247/256    */
    num += s * (__int128)(60 * n2 + 180 * nn + 135); /* (-1)^n terms */

    /* final division by 3840 */
    const __int128 denom = 3840;
    return (seq_t)(num / denom);
}
