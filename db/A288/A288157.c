/*
 * A288157  Number of bases b < n where the digits of n are not all different.
 * https://oeis.org/A288157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of bases b < n such that
   the base‑b representation of n contains a repeated digit.
   Simple O(n log n) algorithm. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n <= 1 or 2 can be handled directly */
    if (n <= 2)
        return (seq_t)0;

    seq_t count = 0;

    /* For each base b from 2 to n-1 */
    for (idx_t b = 2; b < n; ++b)
    {
        idx_t temp = n;
        /* Since n fits in idx_t (uint64_t), at most 64 digits in base 2 */
        idx_t digits[64];
        int len = 0;

        /* Extract digits of n in base b */
        while (temp != 0)
        {
            digits[len++] = temp % b;
            temp /= b;
        }

        /* Check for a repeated digit */
        bool repeated = false;
        for (int i = 0; i < len && !repeated; ++i)
        {
            for (int j = i + 1; j < len; ++j)
            {
                if (digits[i] == digits[j])
                {
                    repeated = true;
                    break;
                }
            }
        }

        if (repeated)
            ++count;
    }

    return count;
}
