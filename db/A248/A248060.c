/*
 * A248060  Sums over successive antidiagonals of A248059.
 * https://oeis.org/A248060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^9 + 18*n^8 + 18*n^7 – 210*n^6 + 588*n^5 + 672*n^4 –
 *            3803*n^3 – 1425*n^2 + 3195*n + 945 + 315*n^3*(-1)^n +
 *            945*n^2*(-1)^n – 315*n*(-1)^n – 945*(-1)^n) /* division by
 *            120960
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 39, u64 n <= 468,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* for offset 1: a(1)=0, a(2)=0, etc.  The formula works for all n>=1. */
    if (n == 0) return 0;          /* safety for unexpected call */

    /* Unsigned 128‑bit intermediate for the product and powers. */
    unsigned __int128 UN = (unsigned __int128)n;

    /* Compute P(n) = 2*n^9 + 18*n^8 + 18*n^7 – 210*n^6 + 588*n^5
                       + 672*n^4 – 3803*n^3 – 1425*n^2 + 3195*n + 945  */
    __int128 P = 2;
    P = P * (__int128)UN + 18;
    P = P * (__int128)UN + 18;
    P = P * (__int128)UN - 210;
    P = P * (__int128)UN + 588;
    P = P * (__int128)UN + 672;
    P = P * (__int128)UN - 3803;
    P = P * (__int128)UN - 1425;
    P = P * (__int128)UN + 3195;
    P = P * (__int128)UN + 945;

    /* Compute Q(n) = 315*n^3 + 945*n^2 – 315*n – 945 */
    __int128 Q = 315;
    Q = Q * (__int128)UN + 945;
    Q = Q * (__int128)UN - 315;
    Q = Q * (__int128)UN - 945;

    /* Add or subtract Q depending on (-1)^n.  (-1)^n = +1 for even n, -1 for odd. */
    __int128 val = ((n & 1ULL) == 0) ? (P + Q) : (P - Q);
    /* val is guaranteed non‑negative for all n>=1. */

    /* Divide by the denominator 120960.  The division is exact. */
    unsigned __int128 out128 = (unsigned __int128)val / 120960;

    return (seq_t)out128;
}
