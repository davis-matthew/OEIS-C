/*
 * A278682  Number of non-equivalent ways to place 3 non-attacking ferses on
 *          an n X n board.
 * https://oeis.org/A278682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n⁶ - 15*n⁴ + 32*n³ + 14*n² - 116*n + 96) +
 *            IF(MOD(n,2)=1, 8*n³ - 9*n² - 20*n + 9))/48
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate arithmetic to avoid overflow for the 64‑bit build.
       The result is then cast to seq_t after division by 48. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 n2  = N * N;          /* n^2 */
    unsigned __int128 n3  = n2 * N;         /* n^3 */
    unsigned __int128 n4  = n3 * N;         /* n^4 */
    unsigned __int128 n6  = n4 * n2;        /* n^6 */

    unsigned __int128 term = n6
                            + (unsigned __int128)32 * n3
                            + (unsigned __int128)14 * n2
                            + (unsigned __int128)96;

    unsigned __int128 sub  = (unsigned __int128)15 * n4
                            + (unsigned __int128)116 * N;
    term -= sub;

    if (n & 1)            /* n is odd */
    {
        term += (unsigned __int128)8  * n3
                - (unsigned __int128)9  * n2
                - (unsigned __int128)20 * N
                + (unsigned __int128)9;
    }

    return (seq_t)(term / 48);
}
