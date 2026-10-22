/*
 * A360518  Numbers j such that there exists a number i <= j with the property
 *          that i+j and i*j have the same decimal digits in reverse order.
 * https://oeis.org/A360518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2 a(2) = 9 a(3) = 24 for n >= 4: a(n) = (10^(n-2))/2 - 3
 *            (derived from a(n) = (10^n - 600)/200, valid for n > 3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)24;

    /* Compute 10^(n-2) then divide by 2 and subtract 3 */
    seq_t pow_val = pow10(n - 2);          /* 10^(n-2) */
    seq_t res = pow_val / 2;               /* divide by 2 */
    res -= (seq_t)3;                       /* subtract 3 */
    return res;
}
