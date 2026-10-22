/*
 * A394910  a(n) = numerator( (2/3)^n*(1/8 - (1/24)*(1/9)^n) ).
 * https://oeis.org/A394910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(0) = 1, a(1) = 13, a(2) = 121, a(n) = 2^(n-3) * ( 3^(2n+1) - 1
 *            ) for n >= 3 (this follows from the closed‑form in the OEIS
 *            entry)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    } else if (n == 1u) {
        return (seq_t)13;
    } else if (n == 2u) {
        return (seq_t)121;
    } else {
        /* n >= 3 */
        seq_t pow2_val = pow2(n - 3u);                     /* 2^(n-3)  */
        seq_t pow3_val = pow3(2u * n + 1u);                /* 3^(2n+1) */
        pow3_val -= (seq_t)1;                              /* subtract 1   */
        return pow2_val * pow3_val;                       /* product      */
    }
}
