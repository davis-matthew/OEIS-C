/*
 * A244870  Number of magic labelings with magic sum n of 2nd graph shown in
 *          link.
 * https://oeis.org/A244870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^5/120 + 7*n^4/16 + 35*n^3/24 + 21*n^2/8 + 149*n/60 +
 *            (-1)^n/32 + 31/32
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a common denominator of 480 to avoid fractional arithmetic */
    unsigned __int128 N = 0;
    unsigned __int128 n128 = (unsigned __int128)n;

    unsigned __int128 n2 = n128 * n128;           /* n^2   */
    unsigned __int128 n3 = n2 * n128;             /* n^3   */
    unsigned __int128 n4 = n3 * n128;             /* n^4   */
    unsigned __int128 n5 = n4 * n128;             /* n^5   */

    N += (unsigned __int128)28 * n5;
    N += (unsigned __int128)210 * n4;
    N += (unsigned __int128)700 * n3;
    N += (unsigned __int128)1260 * n2;
    N += (unsigned __int128)1192 * n128;

    /* add the (-1)^n term: 15*(-1)^n + 465 */
    if (n & 1U) {                 /* n odd */
        N -= (unsigned __int128)15;
    } else {                      /* n even */
        N += (unsigned __int128)15;
    }
    N += (unsigned __int128)465;

    /* divide by the common denominator 480 */
    N /= 480;

    return (seq_t)N;
}
