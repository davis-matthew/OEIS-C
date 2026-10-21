/*
 * A279113  Number of non-equivalent ways to place 4 non-attacking kings on an
 *          n X n board.
 * https://oeis.org/A279113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 54*n^6 + 72*n^5 + 1024*n^4 - 2640*n^3 - 4928*n^2 +
 *            21888*n - 17280 * + IF(n is odd, 14*n^4 - 72*n^3 + 154*n^2 -
 *            240*n - 51, 0)) / 192 * (valid for n ≥ 3; a(1)=0, a(2)=0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 31, u64 n <= 493, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Use the compiler's unsigned 128‑bit type for intermediate calculations.
   This type is available on GCC/Clang; for builds where it is not available
   the compiler will fail, but such builds are outside the intended test set. */
typedef unsigned __int128 uint128_t;

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    seq_t N = n;
    uint128_t N128 = (uint128_t)N;

    uint128_t n2 = N128 * N128;            /* n^2   */
    uint128_t n3 = n2 * N128;              /* n^3   */
    uint128_t n4 = n2 * n2;                /* n^4   */
    uint128_t n5 = n4 * N128;              /* n^5   */
    uint128_t n6 = n4 * n2;                /* n^6   */
    uint128_t n8 = n4 * n4;                /* n^8   */

    uint128_t num = n8
        - (uint128_t)54 * n6
        + (uint128_t)72 * n5
        + (uint128_t)1024 * n4
        - (uint128_t)2640 * n3
        - (uint128_t)4928 * n2
        + (uint128_t)21888 * N128
        - (uint128_t)17280;

    if ((n & 1) == 1) { /* n is odd */
        num += (uint128_t)14 * n4;
        num -= (uint128_t)72 * n3;
        num += (uint128_t)154 * n2;
        num -= (uint128_t)240 * N128;
        num -= (uint128_t)51;
    }

    /* Division by 192; the numerator is guaranteed to be divisible by 192 */
    seq_t result = (seq_t)(num / (uint128_t)192);
    return result;
}
