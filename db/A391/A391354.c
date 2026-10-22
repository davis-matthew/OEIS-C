/*
 * A391354  Numbers k such that 4*k - (greatest prime < 4*k) < (least prime >
 *          4*k) - 4*k.
 * https://oeis.org/A391354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that 4*k - (greatest prime < 4*k) < (least prime
 *            > 4*k) - 4*k
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;

    while (true)
    {
        /* Current composite number to test */
        idx_t m = 4 * k;

        /* Find the greatest prime < m */
        idx_t lower = m - 1;
        while (lower > 1 && !is_prime(lower))
            --lower;

        /* Find the least prime > m */
        idx_t upper = m + 1;
        while (!is_prime(upper))
            ++upper;

        /* Check the inequality */
        if ((m - lower) < (upper - m))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }

        ++k;
    }
}
