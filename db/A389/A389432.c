/*
 * A389432  Smallest nonnegative integer with a constant congruence speed >= n
 *          (see A373387 for the definition of "constant congruence speed").
 * https://oeis.org/A389432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0, a(1) = 2, and for n ≥ 2, a(n) = 2^( n + (1-(-1)^n)/2 )
 *            – ( -1 )^( n·(n+1)/2 ) (i.e. 2^( exp ) plus 1 when n≡1,2 (mod
 *            4), minus 1 when n≡0,3 (mod 4) )
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)2;

    /* compute exponent: n if n even, n+1 if n odd */
    idx_t exp = (n & 1) ? n + 1 : n;

    /* compute (-1)^(n*(n+1)/2)  ->  +1 if n%4==0 or 3, else -1 */
    int sign = (n % 4 == 0 || n % 4 == 3) ? 1 : -1;

    seq_t val = pow2(exp);          /* 2^exp */

    if (sign == 1) {                /* subtract +1 */
        val -= (seq_t)1;
    } else {                        /* subtract -1 == add 1 */
        val += (seq_t)1;
    }
    return val;
}
