/*
 * A399047  Total number of connected components in all induced subgraphs of
 *          the n X 4 rook graph.
 * https://oeis.org/A399047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n + 4*9^n - 4*8^n + 3*7^n - 6*6^n + 2*5^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n;            /* exponent for power calculations */
    seq_t sum = big_pow((seq_t)16, exp);
    sum += (seq_t)4 * big_pow((seq_t)9, exp);
    sum -= (seq_t)4 * big_pow((seq_t)8, exp);
    sum += (seq_t)3 * big_pow((seq_t)7, exp);
    sum -= (seq_t)6 * big_pow((seq_t)6, exp);
    sum += (seq_t)2 * big_pow((seq_t)5, exp);
    return sum;
}
