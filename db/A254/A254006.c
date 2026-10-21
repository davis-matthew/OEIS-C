/*
 * A254006  a(0) = 1, a(n) = 3*a(n-2) if n mod 2 = 0, otherwise a(n) = 0.
 * https://oeis.org/A254006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) if n mod 2 = 0; otherwise a(n) = 0, with a(0) =
 *            1
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 81, u128 n <= 161,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n & 1u)     /* odd indices */
        return 0;
    return pow3(n / 2u);   /* 3^(n/2) for even n */
}
