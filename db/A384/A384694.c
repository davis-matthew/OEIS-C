/*
 * A384694  Sum of the number of cells alive after 2 generations of Conway's
 *          game of life for initial 1 X n cells taken in all 2^n combinations
 *          of alive or dead.
 * https://oeis.org/A384694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 3, a(3) = 12 For n >= 4: a(n) =
 *            2^(n-4) * (11*n - 9)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;
    if (n == 2)
        return 3;
    if (n == 3)
        return 12;

    seq_t coef = ((seq_t)11 * (seq_t)n - (seq_t)9);
    seq_t pow = pow2(n - 4);
    return coef * pow;
}
