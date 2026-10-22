/*
 * A396321  Number of n X n binary matrices with exactly 10 1's such that
 *          there are an even number of 1's in every row and column.
 * https://oeis.org/A396321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2040*C(n,5)^2 + 1440*C(n,5)*C(n,4) + 120*C(n,5)*C(n,3) +
 *            96*C(n,4)^2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 12, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t c5 = binom(n,5);
    seq_t c4 = binom(n,4);
    seq_t c3 = binom(n,3);

    seq_t term1 = (seq_t)2040 * c5 * c5;
    seq_t term2 = (seq_t)1440 * c5 * c4;
    seq_t term3 = (seq_t)120  * c5 * c3;
    seq_t term4 = (seq_t)96   * c4 * c4;

    return term1 + term2 + term3 + term4;
}
