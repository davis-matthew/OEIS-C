/*
 * A381350  Number of subsets of 8 integers between 1 and n such that their
 *          sum is 2 modulo n.
 * https://oeis.org/A381350
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-4)*(2520-24*(281+35*(-1)^n)*n+5*(1039+21*(-1)^n)*n^2
 *            -2112*n^3+452*n^4-48*n^5+2*n^6-2520*A056594(n))/80640 where
 *            A056594(n) = 0 for odd n, and for even n it equals (-1)^(n/2)
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 112, u64 n <= 2575,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 9) return 0;            /* offset is 9, values below are irrelevant */

    /* use signed 128‑bit to avoid overflow for the intermediate values   */
    __int128 N = (__int128)n;
    __int128 n_minus_4 = N - 4;

    /* signs needed for the polynomial and for A056594(n)                  */
    int sg_n   = (n & 1) ? -1 : 1;                          /* (-1)^n          */
    int a_val  = (n & 1)
                  ? 0                            /* odd n             */
                  : (( (n/2) & 1) ? -1 : 1);     /* (-1)^(n/2) for even n */

    /* polynomial part P(n)                                           */
    __int128 P = (__int128)2520
               - (__int128)24 * (281 + 35 * sg_n) * N
               + (__int128)5  * (1039 + 21 * sg_n) * N * N
               - (__int128)2112 * N * N * N
               + (__int128)452  * N * N * N * N
               - (__int128)48   * N * N * N * N * N
               + (__int128)2    * N * N * N * N * N * N;

    /* numerator = (n-4)*(P - 2520*A056594(n))                        */
    __int128 numerator = n_minus_4 * (P - (__int128)2520 * a_val);

    /* exact division by 80640                                         */
    __int128 result = numerator / 80640;

    return (seq_t)result;
}
