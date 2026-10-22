/*
 * A336103  Number of separable multisets of size n covering an initial
 *          interval of positive integers.
 * https://oeis.org/A336103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) - (floor(n/2)+1)*2^(floor(n/2)-2) for n >= 2 *
 *            initial values: a(0) = 1, a(1) = 1, a(2) = 1 * Implementation
 *            uses the utility pow2(idx_t) to compute powers of 2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;               /* a(0)=a(1)=a(2)=1 */

    seq_t pow_n_minus1 = pow2(n - 1);  /* 2^(n-1) */
    idx_t half = n >> 1;               /* floor(n/2) */

    seq_t term;
    if (half == 1)                     /* special case: 2^(1-2)=1/2 */
        term = (seq_t)1;               /* (1+1)*1/2 = 1 */
    else
        term = (seq_t)(half + 1) * pow2(half - 2); /* (half+1)*2^(half-2) */

    return pow_n_minus1 - term;
}
