/*
 * A300761  Number of non-equivalent ways (mod D_2) to select 4 points from n
 *          equidistant points on a straight line so that no selected point is
 *          equally distant from two other selected points.
 * https://oeis.org/A300761
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 12*n^3 + 54*n^2 - 88*n)/48 + (n == 1 (mod
 *            2))*(-4*n + 19)/16 + (n == 5 (mod 6))/3 + (n == 2 (mod 6))/3 +
 *            (n == 2 (mod 4))/2
 * Range:     u8 n <= 13, u16 n <= 45, u32 n <= 676, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute with a signed 128 bit intermediate to avoid overflow
       and to handle negative contributions properly.  The final
       result is guaranteed to be non‑negative, so it can be cast
       back to seq_t. */
    __int128 nn = ( __int128 )n;

    __int128 poly = nn*nn*nn*nn                 /* n^4 */
                 - (__int128)12*nn*nn*nn        /* -12 n^3 */
                 + (__int128)54*nn*nn           /* +54 n^2 */
                 - (__int128)88*nn;            /* -88 n */

    __int128 num = poly;                       /* numerator over 48 */

    if (n & 1) {                               /* odd n: add (-4n+19)/16 --> *3/48 */
        num += (__int128)(-4*nn + 19)*3;
    }

    /* add the c(n) contribution, scaled up to denominator 48 */
    switch (n % 12)
    {
        case 5: case 8: case 11:   /* 1/3  -> 16/48 */
            num += 16;
            break;
        case 6: case 10:          /* 1/2  -> 24/48 */
            num += 24;
            break;
        case 2:                   /* 5/6  -> 40/48 */
            num += 40;
            break;
        default:
            break;               /* 0 contribution */
    }

    return (seq_t)(num / 48);
}
