/*
 * A332134  a(n) = (10^(2n+1)-1)/3 + 10^n.
 * https://oeis.org/A332134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2n+1)-1)/3 + 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute pow10(2*n+1), subtract 1, divide by 3, and add pow10(n) */
    seq_t val = pow10(2 * n + 1);
    val -= 1;
    val /= 3;
    val += pow10(n);
    return val;
}
