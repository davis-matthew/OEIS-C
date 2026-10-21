/*
 * A243143  Number of inequivalent (mod D_3) ways to place 4 points on a
 *          triangular grid of side n so that they are not vertices of an
 *          equilateral triangle of any orientation.
 * https://oeis.org/A243143
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 + 4*n^7 - 14*n^6 - 56*n^5 + 136*n^4 - 104*n^3 +
 *            552*n^2 - 672*n)/2304 + IF(n%2==1)*(28*n^3 - 198*n^2 + 296*n +
 *            21)/768 + IF( (n-1)%4 <= 1 )*(-1/8)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 41, u64 n <= 673,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)            /* values below the offset are irrelevant   */
        return 0;

    /* Use 128‑bit arithmetic internally to avoid overflow of n^8   */
    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 n2 = N * N;           /* n^2   */
    unsigned __int128 n3 = n2 * N;          /* n^3   */
    unsigned __int128 n4 = n3 * N;          /* n^4   */
    unsigned __int128 n5 = n4 * N;          /* n^5   */
    unsigned __int128 n6 = n5 * N;          /* n^6   */
    unsigned __int128 n7 = n6 * N;          /* n^7   */
    unsigned __int128 n8 = n7 * N;          /* n^8   */

    unsigned __int128 first = n8
                             + 4 * n7
                             - 14 * n6
                             - 56 * n5
                             + 136 * n4
                             - 104 * n3
                             + 552 * n2
                             - 672 * N;

    unsigned __int128 numer = first;         /* numerator over 2304            */

    /* Second term (denominator 768) – multiply by 3 to match 2304 */
    if (n & 1) {
        unsigned __int128 second = 28 * n3
                                 - 198 * n2
                                 + 296 * N
                                 + 21;
        numer += second * 3;
    }

    /* Third term: -1/8  (i.e. -288/2304) */
    if ((( n - 1u ) % 4u) <= 1u)
        numer -= 288;

    /* Final division: exact integer division */
    return (seq_t)(numer / 2304u);
}
