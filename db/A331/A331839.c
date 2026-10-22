/*
 * A331839  a(n) = (4^(n + 1) - 2)*(2*n)!.
 * https://oeis.org/A331839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^(n+1) - 2)*(2*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   factorial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* compute (2*n)! */
    idx_t double_n = n + n;          /* 2*n as idx_t */
    seq_t fact = factorial(double_n);

    /* compute 4^(n+1) - 2 */
    seq_t pow4 = pow_int((seq_t)4, n + 1);
    seq_t adjust = pow4 - (seq_t)2;

    /* multiply the two parts */
    return fact * adjust;
}
