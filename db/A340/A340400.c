/*
 * A340400  a(n) is the greatest Fibonacci number < 2^(n+1).
 * https://oeis.org/A340400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the greatest Fibonacci number < 2^(n+1)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Upper bound */
    seq_t threshold = pow2(n + 1);

    /* Fibonacci sequence: F1 = 1, F2 = 1 */
    seq_t f1 = 1;
    seq_t f2 = 1;

    /* Advance until f2 reaches or exceeds threshold */
    while (f2 < threshold) {
        seq_t f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }

    /* f1 is the largest Fibonacci < threshold */
    return f1;
}
