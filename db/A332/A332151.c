/*
 * A332151  a(n) = 5*(10^(2*n+1) - 1)/9 - 4*10^n.
 * https://oeis.org/A332151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1) - 1)/9 - 4*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3) with
   a(0)=1, a(1)=515, a(2)=55155.) */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* power 10^(2*n+1) */
    idx_t exp_high = 2*n + 1;
    seq_t high = pow10(exp_high);

    /* (10^(2*n+1) - 1) / 9 */
    seq_t part = (high - 1) / 9;

    /* multiply by 5 */
    part *= 5;

    /* subtract 4 * 10^n */
    seq_t low = pow10(n);
    seq_t result = part - 4 * low;

    return result;
}
