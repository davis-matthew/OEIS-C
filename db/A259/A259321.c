/*
 * A259321  a(n) = A259110(n)*A259323(n) - A259319(n)^2.
 * https://oeis.org/A259321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4096*n^10-15360*n^8+16128*n^6-5440*n^4+576*n^2)/525
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 68, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for the intermediate sum */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;       /* n^2   */
    unsigned __int128 n4 = n2 * n2;       /* n^4   */
    unsigned __int128 n6 = n4 * n2;       /* n^6   */
    unsigned __int128 n8 = n4 * n4;       /* n^8   */
    unsigned __int128 n10 = n8 * n2;      /* n^10  */

    unsigned __int128 num = 0;
    num += (unsigned __int128)4096 * n10;
    num -= (unsigned __int128)15360 * n8;
    num += (unsigned __int128)16128 * n6;
    num -= (unsigned __int128)5440 * n4;
    num += (unsigned __int128)576  * n2;

    return (seq_t)(num / 525);
}
