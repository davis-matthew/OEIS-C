/*
 * A291354  Numbers m with the property that shifting the rightmost digit of m
 *          to the left end multiplies the number by 6.
 * https://oeis.org/A291354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(4k-3) = 6*(10^(58k)-1)/59, a(4k-2) = 7*(10^(58k)-1)/59,
 *            a(4k-1) = 8*(10^(58k)-1)/59, a(4k) = 9*(10^(58k)-1)/59
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n < 1, return 0 */

    /* Determine the block index k = ceil(n/4) = (n+3)/4   (n >= 1) */
    idx_t k = (n + 3) / 4;

    /* Choose the multiplier based on n mod 4 */
    int r = n % 4;                          /* r in {0,1,2,3} */
    seq_t mult = (r == 1) ? 6 :
                 (r == 2) ? 7 :
                 (r == 3) ? 8 : 9;           /* r==0 => 9 */

    /* Compute (10^(58k) - 1) / 59 and multiply by the chosen multiplier */
    seq_t pow10e = pow10(58 * k);          /* 10^(58k) */
    seq_t val    = pow10e - 1;             /* 10^(58k) - 1 */
    val          = val / 59;               /* division exact for 58k >= 1 */
    val          = val * mult;             /* multiply by 6,7,8,or9 */

    return val;
}
