/*
 * A355998  Number of fixed orthoplex n-ominoes with cell centers determining
 *          (n-2)-space.
 * https://oeis.org/A355998
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-3) * n^(n-5) * (n-2) * (n-3)^2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 4; a(4) = 1 */
    if (n <= 4) {
        return (seq_t)1;
    }
    /* For n >= 5 the formula uses only non‑negative exponents */
    seq_t pow2   = pow_seq((seq_t)2,        (int)(n - 3)); /* 2^(n-3)   */
    seq_t pown   = pow_seq((seq_t)n,        (int)(n - 5)); /* n^(n-5)   */
    seq_t pown3  = pow_seq((seq_t)(n - 3),  2);           /* (n-3)^2   */

    seq_t result = pow2;
    result = result * pown;
    result = result * (seq_t)(n - 2);
    result = result * pown3;
    return result;
}
