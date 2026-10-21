/*
 * A273893  Denominator of n/3^n.
 * https://oeis.org/A273893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, changed
 * Formula:   a(n) = 3^(n - valuation_3(n)) (denominator of n/3^n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* Compute 3-adic valuation of n (exponent of 3 in n) */
    idx_t temp = n;
    idx_t val = 0;
    while (temp % 3 == 0)
    {
        val++;
        temp /= 3;
    }

    /* Result is 3^(n - val) */
    idx_t exp = n - val;
    return pow3(exp);
}
