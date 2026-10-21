/*
 * A258073  a(n) = 1 + 78557*2^n.
 * https://oeis.org/A258073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 78557*2^n (offset 1)
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 47, u128 n <= 111,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    seq_t power = pow2(n);           /* 2^n  */
    seq_t prod  = mul_small(power, 78557); /* 78557 * 2^n */
    return prod + 1;                /* a(n) = 1 + 78557*2^n */
}
