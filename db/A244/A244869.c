/*
 * A244869  Number of magic labelings with magic sum n of first graph shown in
 *          link.
 * https://oeis.org/A244869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*(63 + (-1)^n) + 2592*n + 2880*n^2 + 1660*n^3 +
 *            510*n^4 + 68*n^5) / 960 (Direct evaluation of the polynomial
 *            given in the OEIS entry.)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 140, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, used indirectly */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for multiplication */
    seq_t ns = (seq_t)n;

    /* Compute powers of n in seq_t to keep as arbitrary precision if needed */
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t n4 = n3 * ns;
    seq_t n5 = n4 * ns;

    /* Use a wider intermediate type for the numerator */
    unsigned __int128 num = 0;

    /* 15*(63+(-1)^n) part */
    int sign = (n & 1) ? -1 : 1;      /* (-1)^n */
    int part = 15 * (63 + sign);      /* 930 or 960 */
    num += (unsigned int)part;

    /* Add remaining terms to the numerator */
    num += (unsigned __int128)2592 * (unsigned __int128)ns;
    num += (unsigned __int128)2880 * (unsigned __int128)n2;
    num += (unsigned __int128)1660 * (unsigned __int128)n3;
    num += (unsigned __int128)510  * (unsigned __int128)n4;
    num += (unsigned __int128)68   * (unsigned __int128)n5;

    /* Division by 960 (exact) */
    unsigned __int128 result = num / 960;

    return (seq_t)result;
}
