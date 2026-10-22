/*
 * A350588  a(n) is the number of distinct numbers of steps required for the
 *          last n digits of integers to repeat themselves by iterating the
 *          map m -> m^5.
 * https://oeis.org/A350588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + 2n - 4 - ceil(log2(n)) for n ≥ 9 Initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=6, a(6)=9, a(7)=14, a(8)=23
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   log2ceil_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/log2ceil_idx.h"

seq_t a(idx_t n)
{
    static const seq_t init[8] = {1, 2, 3, 4, 6, 9, 14, 23};

    if (n <= 8)
        return init[n - 1];

    seq_t result = init[7];          /* a(8) */
    for (idx_t k = 9; k <= n; ++k)
    {
        unsigned int ceil_l = log2ceil_idx(k);
        result += (seq_t)(2 * k - 4) - ceil_l;
    }
    return result;
}
