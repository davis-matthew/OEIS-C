/*
 * A291353  Numbers m with the property that shifting the rightmost digit of m
 *          to the left end multiplies the number by 9.
 * https://oeis.org/A291353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9*(10^(44*n)-1)/89
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute e = 44*n.  n ≥ 1, so the product fits in idx_t. */
    idx_t e = 44 * n;

    /* 10^e as a seq_t value. */
    seq_t p = pow10(e);

    /* Follow the explicit formula. */
    seq_t result = (p - 1) / 89;  /* 10^(44*n)-1 is always divisible by 89 */
    result = result * 9;
    return result;
}
