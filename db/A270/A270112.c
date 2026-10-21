/*
 * A270112  Number of n X 2 0..7 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling seven
 *          exactly once.
 * https://oeis.org/A270112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 8 * a(2) = 1344 * for n > 2: a(n) = 1344 * 43^(n-3) *
 *            (32*n - 21)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)8;
    if (n == 2)
        return (seq_t)1344;

    /* Compute 43^(n-3) */
    seq_t exp = (seq_t)(n - 3);
    seq_t pow43 = big_pow((seq_t)43, exp);

    /* Multiply by 1344 and (32*n - 21) */
    seq_t coeff = ((seq_t)n * (seq_t)32) - (seq_t)21;
    seq_t result = pow43 * (seq_t)1344 * coeff;

    return result;
}
