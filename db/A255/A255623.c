/*
 * A255623  Number of length n+1 0..2 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^8 + 44*n^7 + 490*n^6 + 1568*n^5 + 5929*n^4 + 28196*n^3
 *            + 74220*n^2 + 131472*n + 120960) / 40320 (OEIS A255623)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 55, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* use 128‑bit arithmetic to avoid overflow for n up to about 10 000 */
    __int128 nn = n;            /* base value */
    __int128 n2 = nn * nn;      /* n^2 */
    __int128 n3 = n2 * nn;      /* n^3 */
    __int128 n4 = n3 * nn;      /* n^4 */
    __int128 n5 = n4 * nn;      /* n^5 */
    __int128 n6 = n5 * nn;      /* n^6 */
    __int128 n7 = n6 * nn;      /* n^7 */
    __int128 n8 = n7 * nn;      /* n^8 */

    __int128 num = n8;
    num += (__int128)44   * n7;
    num += (__int128)490  * n6;
    num += (__int128)1568 * n5;
    num += (__int128)5929 * n4;
    num += (__int128)28196* n3;
    num += (__int128)74220* n2;
    num += (__int128)131472* nn;
    num += (__int128)120960;          /* constant term */

    /* divide by 40320 (exact division for all integer n) */
    seq_t result = (seq_t)(num / (__int128)40320);
    return result;
}
