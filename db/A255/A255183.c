/*
 * A255183  Third differences of ninth powers (A001017).
 * https://oeis.org/A255183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 509 a(n>=2) = 6*(84*n^6 - 252*n^5 + 630*n^4 -
 *            840*n^3 + 756*n^2 - 378*n + 85)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 14, u64 n <= 576, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 509;

    /* use a signed 128‑bit intermediate to avoid overflow and negative values */
    __int128 nn = n;
    __int128 r  = 84;

    r = r * nn - 252;
    r = r * nn + 630;
    r = r * nn - 840;
    r = r * nn + 756;
    r = r * nn - 378;
    r = r * nn + 85;
    r = r * 6;

    return (seq_t)r;
}
