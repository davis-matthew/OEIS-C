/*
 * A382675  a(n) = 4 - 15 * 2^n + 12 * 3^n.
 * https://oeis.org/A382675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   mul_small, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4 - 15 * 2^n + 12 * 3^n   (OEIS A382675)
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* compute 15 * 2^n */
    seq_t pow2n  = pow2(n);          /* 2^n   */
    seq_t term1  = mul_small(pow2n, 15); /* 15*2^n */

    /* compute 12 * 3^n */
    seq_t pow3n  = pow3(n);          /* 3^n   */
    seq_t term2  = mul_small(pow3n, 12); /* 12*3^n */

    /* 4 + term2 - term1  (keeps all operations in seq_t) */
    seq_t result = term2 + (seq_t)4;
    result -= term1;

    return result;
}
