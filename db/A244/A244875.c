/*
 * A244875  Number of magic labelings with magic sum n of 7th graph shown in
 *          link.
 * https://oeis.org/A244875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (315*(255+(-1)^n) + 306144*n + 498456*n^2 + 452312*n^3 +
 *            250530*n^4 + 86576*n^5 + 17724*n^6 + 1688*n^7) / 80640
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 39, u64 n <= 980, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for safe intermediate values. */
    typedef __int128 int128_t;
    int128_t N = (int128_t)n;

    /* (-1)^n */
    int sign = (n & 1) ? -1 : 1;

    /* Pre‑compute powers of n. */
    int128_t n1 = N;
    int128_t n2 = n1 * n1;
    int128_t n3 = n2 * n1;
    int128_t n4 = n3 * n1;
    int128_t n5 = n4 * n1;
    int128_t n6 = n5 * n1;
    int128_t n7 = n6 * n1;

    /* Compute numerator. */
    int128_t numerator =
        (int128_t)315 * ((int128_t)255 + (int128_t)sign) +
        (int128_t)306144 * n1 +
        (int128_t)498456 * n2 +
        (int128_t)452312 * n3 +
        (int128_t)250530 * n4 +
        (int128_t)86576  * n5 +
        (int128_t)17724  * n6 +
        (int128_t)1688   * n7;

    /* Divide by 80640 and cast back to seq_t. */
    seq_t result = (seq_t)(numerator / (int128_t)80640);
    return result;
}
