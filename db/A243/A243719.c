/*
 * A243719  Number of inequivalent (mod D_8) ways to place 4 nonattacking
 *          knights on an n X n board.
 * https://oeis.org/A243719
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 54*n^6 + 144*n^5 + 1048*n^4 - 5280*n^3 - 2432*n^2
 *            * + 52800*n - 78912 * + (1-(-1)^n)/2*(14*n^4 - 48*n^3 - 158*n^2
 *            + 768*n - 723)) / 192 * for n ≥ 6. For 2 ≤ n < 6 the values are:
 *            1, 6, 66, 609
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 31, u64 n <= 493, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset: a(2)=1, a(3)=6, a(4)=66, a(5)=609 */
    if (n < 2) return 0;          /* undefined for n < 2, return 0 */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)66;
    if (n == 5) return (seq_t)609;

    /* compute with unsigned __int128 intermediates to avoid overflow */
    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 n2 = N * N;          /* n^2 */
    unsigned __int128 n3 = n2 * N;         /* n^3 */
    unsigned __int128 n4 = n3 * N;         /* n^4 */
    unsigned __int128 n5 = n4 * N;         /* n^5 */
    unsigned __int128 n6 = n5 * N;         /* n^6 */
    unsigned __int128 n8 = n6 * n2;        /* n^8 = n^6 * n^2 */

    /* positive part of numerator */
    unsigned __int128 pos = n8;
    pos += (unsigned __int128)1048 * n4;
    pos += (unsigned __int128)144 * n5;
    pos += (unsigned __int128)52800 * N;

    /* negative part of numerator */
    unsigned __int128 neg = (unsigned __int128)54 * n6;
    neg += (unsigned __int128)5280 * n3;
    neg += (unsigned __int128)2432 * n2;
    neg += (unsigned __int128)78912;

    /* additional terms for odd n */
    if (n & 1U) {
        unsigned __int128 extra = (unsigned __int128)14 * n4;
        extra -= (unsigned __int128)48 * n3;
        extra -= (unsigned __int128)158 * n2;
        extra += (unsigned __int128)768 * N;
        extra -= (unsigned __int128)723;
        pos += extra;
    }

    /* numerator of the formula */
    unsigned __int128 numer = pos - neg;

    /* divide by 192 and cast back to seq_t */
    return (seq_t)(numer / (unsigned __int128)192);
}
