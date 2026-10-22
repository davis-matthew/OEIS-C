/*
 * A338955  Number of achiral colorings of the 96 edges (or triangular faces)
 *          of the 4-D 24-cell using subsets of a set of n colors.
 * https://oeis.org/A338955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^16 + 8*n^18 + 16*n^20 + 12*n^24 + 2*n^48 + n^52 +
 *            n^60) / 48
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset starts at 1 */

    seq_t x = (seq_t)n;

    seq_t n16 = big_pow(x, 16);
    seq_t n18 = big_pow(x, 18);
    seq_t n20 = big_pow(x, 20);
    seq_t n24 = big_pow(x, 24);
    seq_t n48 = big_pow(x, 48);
    seq_t n52 = big_pow(x, 52);
    seq_t n60 = big_pow(x, 60);

    seq_t num = 8 * n16 + 8 * n18 + 16 * n20 + 12 * n24 + 2 * n48 + n52 + n60;

    return num / 48;
}
