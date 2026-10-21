/*
 * A261547  The 3 X 3 X ... X 3 dots problem (3, n times): minimal number of
 *          straight lines (connected at their endpoints) required to pass
 *          through 3^n dots arranged in a 3 X 3 X ... X 3 grid.
 * https://oeis.org/A261547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^n - 1)/2 for n >= 1, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t p = big_pow((seq_t)3, (seq_t)n);
    return (p - (seq_t)1) / (seq_t)2;
}
