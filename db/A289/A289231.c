/*
 * A289231  Number of nonequivalent ways to select 4 disjoint point triples
 *          from an n X n X n triangular point grid, each point triple forming
 *          a 2 X 2 X 2 triangle.
 * https://oeis.org/A289231
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 31, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A289231: Number of nonequivalent ways to select 4 disjoint point triples
   from an n×n×n triangular point grid.  Formula (from OEIS):
      a(n) = (n^8 -8*n^7 -50*n^6 +556*n^5 +261*n^4 -12724*n^3 +19088*n^2
              +86016*n -201024)/144
             + IF(n mod 2 = 1, (-2*n +5)/4, 0)
             + IF(n mod 3 = 1, (-n^2 +2*n +12)/9, 0)
   (n ≥ 4; terms for n < 4 are undefined – this function returns 0.)
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                         /* undefined for n < 4 */

    /* use signed 128‑bit for all intermediate arithmetic */
    __int128 nn   = n;                    /* n                     */
    __int128 n2   = nn * nn;              /* n^2                   */
    __int128 n3   = n2 * nn;              /* n^3                   */
    __int128 n4   = n2 * n2;              /* n^4                   */
    __int128 n5   = n4 * nn;              /* n^5                   */
    __int128 n6   = n3 * n3;              /* n^6                   */
    __int128 n7   = n6 * nn;              /* n^7                   */
    __int128 n8   = n4 * n4;              /* n^8                   */

    /* main rational part (denominator 144) */
    __int128 main_num =
        n8
        - (__int128)8   * n7
        - (__int128)50  * n6
        + (__int128)556 * n5
        + (__int128)261 * n4
        - (__int128)12724 * n3
        + (__int128)19088 * n2
        + (__int128)86016 * nn
        - (__int128)201024;

    /* odd n correction: (-2*n + 5)/4 => multiply by 36 to stay over 144 */
    if (n & 1)
        main_num += (__int128)(-2 * nn + 5) * 36;

    /* n ≡ 1 (mod 3) correction: (-n^2 + 2*n + 12)/9 => multiply by 16 */
    if (n % 3 == 1)
        main_num += (__int128)(-n2 + 2 * nn + 12) * 16;

    /* integer division by 144 – exact for all n ≥ 4 */
    return (seq_t)(main_num / 144);
}
