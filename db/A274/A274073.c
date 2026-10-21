/*
 * A274073  a(n) = 6^n-(-1)^n.
 * https://oeis.org/A274073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - (-1)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    if ((n & 1) == 0) {
        /* n is even: (-1)^n == 1  → subtract 1 */
        pow6 -= (seq_t)1;
    } else {
        /* n is odd: (-1)^n == -1 → subtract (-1) == add 1 */
        pow6 += (seq_t)1;
    }
    return pow6;
}
