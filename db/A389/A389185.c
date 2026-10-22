/*
 * A389185  Number of mutual-visibility sets in the n-Moebius ladder.
 * https://oeis.org/A389185
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^7 + 154*n^6 + 2548*n^5 + 47320*n^4 - 58016*n^3 +
 *            9856*n^2 + 2449152*n + 161280) / 161280 (even n >= 6) a(n) =
 *            (n^7 + 196*n^6 + 3913*n^5 + 67480*n^4 + 189259*n^3 - 3212636*n^2
 *            + 12144747*n + 161280) / 161280 (odd n >= 9)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 112, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    if (n == 3) return 57;
    if (n == 4) return 127;
    if (n == 5) return 383;
    if (n == 7) return 1527;

    /* Compute powers of n up to n^7 using 128‑bit arithmetic */
    __uint128_t sn = (__uint128_t)n;
    __uint128_t p2 = sn * sn;          /* n^2   */
    __uint128_t p3 = p2 * sn;          /* n^3   */
    __uint128_t p4 = p3 * sn;          /* n^4   */
    __uint128_t p5 = p4 * sn;          /* n^5   */
    __uint128_t p6 = p5 * sn;          /* n^6   */
    __uint128_t p7 = p6 * sn;          /* n^7   */

    const __uint128_t denom = 161280;
    __uint128_t numer;

    if ((n & 1) == 0) /* even n >= 6 */
    {
        numer = p7
              + (__uint128_t)154   * p6
              + (__uint128_t)2548  * p5
              + (__uint128_t)47320 * p4
              + (__uint128_t)9856  * p2
              + (__uint128_t)2449152 * sn
              + (__uint128_t)161280;
        numer -= (__uint128_t)58016 * p3;
    }
    else /* odd n >= 9 */
    {
        numer = p7
              + (__uint128_t)196   * p6
              + (__uint128_t)3913  * p5
              + (__uint128_t)67480 * p4
              + (__uint128_t)189259 * p3
              + (__uint128_t)12144747 * sn
              + (__uint128_t)161280;
        numer -= (__uint128_t)3212636 * p2;
    }

    return (seq_t)(numer / denom);
}
