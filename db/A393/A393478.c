/*
 * A393478  Number of mutual-visibility sets in the n-Plummer-Toft graph.
 * https://oeis.org/A393478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23 + 53*2^(n-2) + 3*n (for n > 2) a(1) = 54 a(2) = 81
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 54;
    if (n == 2)
        return 81;
    /* n > 2 */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 2));
    seq_t res = 23;
    res += 53 * pow2;
    res += 3 * (seq_t)n;
    return res;
}
