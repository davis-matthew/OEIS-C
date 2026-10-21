/*
 * A248016  Sum over each antidiagonal of A248011.
 * https://oeis.org/A248016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^7 + 14*n^6 + 14*n^5 + 70*n^4 - 77*n^3 - 399*n^2 +
 *            61*n + 105 - 105*(-1)^n - 35*n^3*(-1)^n - 105*n^2*(-1)^n +
 *            35*n*(-1)^n)/6720
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 74, u64 n <= 1802,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    /* Use signed 128‑bit to avoid overflow of the polynomial terms. */
    __int128 n128 = n;
    __int128 n2 = n128 * n128;
    __int128 n3 = n2 * n128;
    __int128 n4 = n3 * n128;
    __int128 n5 = n4 * n128;
    __int128 n6 = n5 * n128;
    __int128 n7 = n6 * n128;

    int sign = (n & 1) ? -1 : 1;  /* (-1)^n */

    __int128 num = 0;
    num += 2 * n7;
    num += 14 * n6;
    num += 14 * n5;
    num += 70 * n4;
    num -= 77 * n3;
    num -= 399 * n2;
    num += 61 * n128;
    num += 105;

    if (sign < 0) {           /* n is odd */
        num += 105;           /* -105*(-1)^n  -> +105 */
        num += 35 * n3;       /* -35*n^3*(-1)^n -> +35*n^3 */
        num += 105 * n2;      /* -105*n^2*(-1)^n -> +105*n^2 */
        num -= 35 * n128;     /* +35*n*(-1)^n  -> -35*n */
    } else {                  /* n is even */
        num -= 105;
        num -= 35 * n3;
        num -= 105 * n2;
        num += 35 * n128;
    }

    num /= 6720;              /* exact division */

    return (seq_t)num;
}
