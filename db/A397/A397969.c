/*
 * A397969  Number of inequivalent ways to choose 4 squares on an n X n board
 *          with no chosen square orthogonally adjacent to another, with
 *          equivalence being up to rotations.
 * https://oeis.org/A397969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (n^8-30*n^6+24*n^5+326*n^4-504*n^3-1134*n^2+2784*n-1179)/96 for
 *            odd n > 2, a(n) =
 *            (n^8-30*n^6+24*n^5+326*n^4-504*n^3-1128*n^2+2784*n-1248)/96 for
 *            even n > 3, a(1) = 0, a(2) = 0 computed directly in O(1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 28, u64 n <= 452,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 nn2 = nn * nn;          /* n^2  */
    unsigned __int128 nn3 = nn2 * nn;         /* n^3  */
    unsigned __int128 nn4 = nn3 * nn;         /* n^4  */
    unsigned __int128 nn5 = nn4 * nn;         /* n^5  */
    unsigned __int128 nn6 = nn5 * nn;         /* n^6  */
    unsigned __int128 nn7 = nn6 * nn;         /* n^7  */
    unsigned __int128 nn8 = nn7 * nn;         /* n^8  */

    unsigned __int128 numer;
    if (n & 1) { /* odd n > 2 */
        numer =
            nn8
            - (unsigned __int128)30 * nn6
            + (unsigned __int128)24 * nn5
            + (unsigned __int128)326 * nn4
            - (unsigned __int128)504 * nn3
            - (unsigned __int128)1134 * nn2
            + (unsigned __int128)2784 * nn
            - (unsigned __int128)1179;
    } else {      /* even n > 3 */
        numer =
            nn8
            - (unsigned __int128)30 * nn6
            + (unsigned __int128)24 * nn5
            + (unsigned __int128)326 * nn4
            - (unsigned __int128)504 * nn3
            - (unsigned __int128)1128 * nn2
            + (unsigned __int128)2784 * nn
            - (unsigned __int128)1248;
    }

    numer /= 96;   /* exact division */

    return (seq_t) numer;
}
