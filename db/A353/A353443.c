/*
 * A353443  Integers m such that the decimal expansion of 1/m contains the
 *          digit 7.
 * https://oeis.org/A353443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer m such that the decimal expansion of 1/m contains the digit 7 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m)
    {
        idx_t r = 1 % m;
        bool has7 = false;

        for (idx_t step = 0; step < m && !has7; ++step)
        {
            idx_t digit = (r * 10) / m;
            if (digit == 7)
            {
                has7 = true;
                break;
            }
            r = (r * 10) % m;
            if (r == 0)
                break;          /* terminating decimal – no further digits can be 7 */
        }

        if (has7)
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
