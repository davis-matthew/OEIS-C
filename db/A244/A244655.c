/*
 * A244655  Least number k such that 3^k ends in exactly n consecutive
 *          decreasing digits.
 * https://oeis.org/A244655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(1)=1, a(2)=5, a(3)=27, a(n)=5*10^(n-2)-4 for n>=4 (pattern: 4
 *            followed by n-2 nines, then 6)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 5;
    if (n == 3) return 27;
    /* n >= 4 */
    idx_t e = n - 2;
    seq_t p = pow10(e);          /* p = 10^(n-2) */
    seq_t res = p * 5;
    res -= 4;
    return res;
}
