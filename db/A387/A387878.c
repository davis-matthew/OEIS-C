/*
 * A387878  a(n) = Sum_{k=0..n} binomial(6*n+4,6*k).
 * https://oeis.org/A387878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1 + 16*64^n - 9*(-27)^n)/6
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t pow64 = pow_uint(64, n);
    seq_t pow27 = pow_uint(27, n);
    seq_t term1 = mul_small(pow64, 16);   /* 16*64^n */
    seq_t term2 = mul_small(pow27, 9);    /* 9*27^n */
    seq_t numerator;

    if ((n & 1ULL) == 0) {               /* even n: (-27)^n = 27^n  */
        numerator = term1 - term2 - 1;   /* -1 + term1 - term2 */
    } else {                              /* odd n: (-27)^n = -27^n */
        numerator = term1 + term2 - 1;   /* -1 + term1 + term2 */
    }
    return numerator / 6;
}
