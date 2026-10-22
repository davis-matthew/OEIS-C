/*
 * A327794  The number of (n-2)-interval parking functions of size n.
 * https://oeis.org/A327794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2 * 3^(n-2) + 2*n - 3 for n >= 4 a(1) = a(2) = 0, a(3) =
 *            6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return 0;
    if (n == 3)
        return 6;

    seq_t pow3 = seq_pow((seq_t)3, n - 2);
    seq_t result = 2 * pow3;

    seq_t add = (seq_t)(2 * n - 3);
    result += add;

    return result;
}
