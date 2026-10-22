/*
 * A393662  A sequence arising from the 3-Bratteli diagram, a(n) = 3^(n -
 *          3)*(16*n - 15), n>=3.
 * https://oeis.org/A393662
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n - 3)*(16*n - 15)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t factor = (seq_t)16 * n - (seq_t)15;
    return pow3(n - 3) * factor;
}
