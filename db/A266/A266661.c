/*
 * A266661  Decimal representation of the n-th iteration of the "Rule 47"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 6; for n ≥ 2, * if n is even a(n) = 3, * if n
 *            is odd a(n) = 2*4^n - 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    /* n ≥ 2 */
    if ((n & 1) == 0)       /* even */
        return (seq_t)3;

    /* odd n > 1 */
    seq_t val = pow2((idx_t)(2 * n));   /* 4^n = 2^(2n) */
    val = val * (seq_t)2;               /* 2 * 4^n */
    val = val - (seq_t)4;               /* 2*4^n - 4 */
    return val;
}
