/*
 * A269290  Number of 2 X n 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          no more than once.
 * https://oeis.org/A269290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 16 a(2) = 216 a(3) = 2124 a(4) = 19188 a(n) = 36 *
 *            7^(n-4) * (120*n + 53) for n > 4 (the recurrence a(n) =
 *            14*a(n-1) - 49*a(n-2) for n>4 is equivalent) This implements the
 *            closed‑form and base cases directly
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of OEIS offset – undefined */
    if (n == 1) return 16;
    if (n == 2) return 216;
    if (n == 3) return 2124;
    if (n == 4) return 19188;

    seq_t pow7 = big_pow((seq_t)7, (seq_t)(n - 4));
    seq_t term = (seq_t)120 * (seq_t)n + (seq_t)53;
    seq_t result = (seq_t)36;
    result *= pow7;
    result *= term;
    return result;
}
