/*
 * A343029  Number of 1-bits in the binary expansion of n which have an even
 *          number of 0-bits at less significant bit positions.
 * https://oeis.org/A343029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of 1-bits in the binary expansion of n that have *
 *            an even number of 0-bits in less significant positions
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t ones_so_far = 0;   /* number of 1-bits seen in positions < current */
    idx_t pos = 0;           /* current bit position, starting at 0 (LSB) */
    idx_t cur = n;

    while (cur != 0)
    {
        if (cur & 1ULL)
        {
            /* zeros below this bit = pos - ones_so_far.
             * Even iff (pos & 1) == (ones_so_far & 1) */
            if ((pos & 1ULL) == (ones_so_far & 1ULL))
                res++;

            ones_so_far++;
        }
        cur >>= 1;
        pos++;
    }

    return res;
}
