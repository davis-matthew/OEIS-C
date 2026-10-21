/*
 * A276274  a(n) = (2*n+1)^(2*(2*n+1)^2).
 * https://oeis.org/A276274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)^(2*(2*n+1)^2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute m = 2*n + 1 */
    seq_t m = (seq_t)(2 * n + 1);
    /* exponent = 2 * m * m */
    seq_t exp = (seq_t)(2 * m * m);
    /* return m^exp using the provided big_pow routine */
    return big_pow(m, exp);
}
