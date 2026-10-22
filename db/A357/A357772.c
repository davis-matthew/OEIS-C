/*
 * A357772  Numbers with a sum of digits which is not 7-smooth.
 * https://oeis.org/A357772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = nth positive integer whose digit sum is not 7‑smooth
 *            (digit sum computed via digit_sum(), smoothness checked by
 *            not_smooth_7)
 * Range:     u8 n <= 45, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_sum, not_smooth_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/not_smooth_7.h"

seq_t a(idx_t n)
{
    /* idx_t is unsigned; n >= 1 by contract. */
    seq_t cur = 1;
    idx_t count = 0;

    while (1)
    {
        seq_t sum = digit_sum(cur);
        uint64_t s = (uint64_t)sum;        /* digit sum is tiny, fits in 64 bits */
        if (not_smooth_7(s))
        {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
