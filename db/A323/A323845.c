/*
 * A323845  Number of inequivalent height 1 degree n polynomials with nonzero
 *          constant term.
 * https://oeis.org/A323845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k+1) = 3^k*(3^k+1)/2, a(2k+2) = 3^k*(3^(k+1)+5)/2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (n & 1) { /* odd n = 2k+1 */
        idx_t k = n >> 1;                /* k = floor(n/2) = (n-1)/2 */
        seq_t p = big_pow(3, (seq_t)k);   /* p = 3^k */
        return (p * (p + 1)) / 2;         /* (3^k * (3^k+1)) / 2 */
    } else { /* even n = 2k+2 */
        idx_t k = (n >> 1) - 1;           /* k = n/2 - 1 = (n-2)/2 */
        seq_t p = big_pow(3, (seq_t)k);   /* p = 3^k */
        seq_t p_next = p * 3;             /* 3^(k+1) */
        return (p * (p_next + 5)) / 2;    /* (3^k * (3^(k+1)+5)) / 2 */
    }
}
