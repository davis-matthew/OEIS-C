/*
 * A243467  Number of ways 5 domicules can be placed on an n X n square.
 * https://oeis.org/A243467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5464830 -2069823*n -2896628*n^2 +1279635*n^3 +493090*n^4
 *            * -285192*n^5 -22560*n^6 +27360*n^7 -1600*n^8 -960*n^9
 *            +128*n^10)/15 * for n >= 5; * a(4) = 33792; a(n) = 0 for n <= 3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 68, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4ULL) {
        return (seq_t)0;
    }
    if (n == 4ULL) {
        return (seq_t)33792;
    }

    /* Use signed __int128 for the intermediates to avoid overflow
     * up to n ~ 10,000 when the result fits in seq_t.  The coefficients
     * are applied using Horner's rule:
     * ((((((((128*n-960)*n-1600)*n+27360)*n-22560)*n-285192)*n+493090)
     *   *n+1279635)*n-2896628)*n-2069823)*n+5464830) / 15
     */
    __int128 s = ( __int128 )n;
    __int128 res = (__int128)128;

    res = res * s + ( __int128)(-960);
    res = res * s + ( __int128)(-1600);
    res = res * s + ( __int128)27360;
    res = res * s + ( __int128)(-22560);
    res = res * s + ( __int128)(-285192);
    res = res * s + ( __int128)493090;
    res = res * s + ( __int128)1279635;
    res = res * s + ( __int128)(-2896628);
    res = res * s + ( __int128)(-2069823);
    res = res * s + ( __int128)5464830;

    res /= (__int128)15;

    return (seq_t)res;
}
