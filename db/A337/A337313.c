/*
 * A337313  a(n) is the number of n-digit positive integers with exactly three
 *          distinct base 10 digits.
 * https://oeis.org/A337313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 324*(3^(n-1) - 2^n + 1) (n ≥ 3, a(1)=a(2)=0)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;
    }
    seq_t term = pow3(n - 1);   /* 3^(n-1) */
    term -= pow2(n);            /* subtract 2^n   */
    term += (seq_t)1;           /* add 1          */
    term *= (seq_t)324;         /* multiply by 324 */
    return term;
}
