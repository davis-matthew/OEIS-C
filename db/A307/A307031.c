/*
 * A307031  n to the power n double factorial, n^(n!!).
 * https://oeis.org/A307031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n!!)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n!! : product of n, n-2, n-4, … until 1 or 2 */
    seq_t d = (seq_t)1;
    for (idx_t i = n; i > 1; i -= 2)
        d *= (seq_t)i;

    return big_pow((seq_t)n, d);
}
