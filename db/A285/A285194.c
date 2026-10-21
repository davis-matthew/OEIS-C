/*
 * A285194  Expansion of (1+x^2)/(1+x+x^4) mod 3.
 * https://oeis.org/A285194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [1,2,2,1,1,0,1,1,1,2,0,2,0][n mod 13]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    switch ((size_t)(n % 13))
    {
        case 0:  return 1;
        case 1:  return 2;
        case 2:  return 2;
        case 3:  return 1;
        case 4:  return 1;
        case 5:  return 0;
        case 6:  return 1;
        case 7:  return 1;
        case 8:  return 1;
        case 9:  return 2;
        case 10: return 0;
        case 11: return 2;
        case 12: return 0;
        default: return 0;   /* never reached */
    }
}
