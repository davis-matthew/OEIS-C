/*
 * A309675  a(n) = 4^n^2 + n!.
 * https://oeis.org/A309675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n^2) + n!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   fact_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/fact_upto.h"

seq_t a(idx_t n)
{
    /* compute 4^(n^2) + n! */
    seq_t pow4 = pow_uint(4, n * n);
    seq_t fact = fact_upto(n);
    return pow4 + fact;
}
