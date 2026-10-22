/*
 * A345963  a(n) = (q^2-q+1)/3 where q = 2^(2*n+1) = A004171(n).
 * https://oeis.org/A345963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (q^2 - q + 1)/3 where q = 2^(2*n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* q = 2^(2*n+1) */
    idx_t exp = 2*n + 1;
    seq_t q = pow2(exp);

    /* q^2 */
    seq_t q2 = q * q;

    /* (q^2 - q + 1) / 3 */
    seq_t temp = q2 - q + 1;
    return temp / 3;
}
