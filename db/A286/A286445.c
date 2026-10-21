/*
 * A286445  Number of non-equivalent ways to tile an n X n X n triangular area
 *          with three 2 X 2 X 2 triangular tiles and an appropriate number (=
 *          n^2-12) of 1 X 1 X 1 tiles.
 * https://oeis.org/A286445
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 -9*n^5 +6*n^4 +165*n^3 -447*n^2 -372*n +1736)/36 +
 *            IF(MOD(n, 2) = 1, -n^2 +6*n -9)/2 + IF(MOD(n, 3) = 0, -2)/9 for
 *            n >= 4. For n = 3 the value is 0
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 74, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)0;
    /* The calculation is performed in unsigned __int128 so it works
       for n up to about 10 000 without overflow.  The final result
       fits in the build‑dependent seq_t type. */
    unsigned __int128 n128 = n;
    unsigned __int128 n2 = n128 * n128;
    unsigned __int128 n3 = n2 * n128;
    unsigned __int128 n4 = n3 * n128;
    unsigned __int128 n5 = n4 * n128;
    unsigned __int128 n6 = n5 * n128;
    unsigned __int128 N =
        n6
        - (unsigned __int128)9 * n5
        + (unsigned __int128)6 * n4
        + (unsigned __int128)165 * n3
        - (unsigned __int128)447 * n2
        - (unsigned __int128)372 * n128
        + (unsigned __int128)1736;

    if (n & 1) {                /* n is odd: add (-n^2+6n-9)/2   */
        unsigned __int128 t   = n128 - 3;          /* (n-3)^2 = n^2-6n+9   */
        unsigned __int128 adj = t * t * 18;       /* 18 * -(n^2-6n+9)   */
        N -= adj;                               /* subtract because expression is negative */
    }

    if ((n % 3) == 0) {          /* n divisible by 3: add (-2)/9   */
        N -= 8;                  /* 8 = 36 * (2/9)   */
    }

    return (seq_t)(N / 36);
}
