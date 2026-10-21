/*
 * A289228  Number of ways to select 7 disjoint point triples from an n X n X
 *          n triangular point grid, each point triple forming a 2 X 2 X 2
 *          triangle.
 * https://oeis.org/A289228
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^14 - 14*n^13 - 182*n^12 + 3416*n^11 + 9072*n^10 * -
 *            342062*n^9 + 296688*n^8 + 17893944*n^7 - 48845153*n^6 * -
 *            511039228*n^5 + 2041220174*n^4 + 7429535400*n^3 * -
 *            37737333320*n^2 - 41483946096*n + 262680697440) / 5040 * for n
 *            >= 7, a(6) = 0. Rotations and reflections of a selection are
 *            r...
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 11, u64 n <= 45, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(6) is the first entry (offset 6) – all smaller n are invalid */
    if (n < 7)
        return (seq_t)0;

    /* use unsigned __int128 for intermediate arithmetic to avoid overflow
     * in the 64‑bit build while remaining correct for the arbitrary‑precision
     * build (the cast at the end converts the final value to seq_t).
     */
    unsigned __int128 pow[15];
    pow[0] = (unsigned __int128)1;
    for (int i = 1; i <= 14; ++i)
        pow[i] = pow[i - 1] * (unsigned __int128)n;

    unsigned __int128 num = 0;

    /* Positive terms */
    num += pow[14];
    num += pow[11] * (unsigned __int128)3416;
    num += pow[10] * (unsigned __int128)9072;
    num += pow[8]  * (unsigned __int128)296688;
    num += pow[7]  * (unsigned __int128)17893944;
    num += pow[4]  * (unsigned __int128)2041220174;
    num += pow[3]  * (unsigned __int128)7429535400;
    num += (unsigned __int128)262680697440;

    /* Negative terms */
    num -= pow[13] * (unsigned __int128)14;
    num -= pow[12] * (unsigned __int128)182;
    num -= pow[9]  * (unsigned __int128)342062;
    num -= pow[6]  * (unsigned __int128)48845153;
    num -= pow[5]  * (unsigned __int128)511039228;
    num -= pow[2]  * (unsigned __int128)37737333320;
    num -= pow[1]  * (unsigned __int128)41483946096;

    /* Final division */
    num /= (unsigned __int128)5040;

    return (seq_t)(num);
}
