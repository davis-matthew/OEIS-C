/*
 * A302489  Total domination number of the n-cube connected cycle graph.
 * https://oeis.org/A302489
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A302489:  a(n) = 2^(n-3) * (2n + 4)  for n >= 4,
   and the special value a(3) = 8. */

#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 3) {
        return (seq_t)8;
    }
    seq_t pow_two = pow_uint(2, n - 3);          /* 2^(n-3) */
    seq_t factor = (seq_t)(2 * n + 4);           /* (2n + 4)   */
    return seq_mul(pow_two, factor);            /* product     */
}
