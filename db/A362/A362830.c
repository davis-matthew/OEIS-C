/*
 * A362830  Number of bases b with 2 <= b < n such that n written in base b is
 *          a strictly increasing sequence of digits.
 * https://oeis.org/A362830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute force: For each 2 ≤ b < n, check if the representation
   of n in base b has strictly increasing digits (most‑significant to least).
   Returns the number of such bases. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t b = 2; b < n; ++b)
    {
        idx_t temp = n;
        bool first = true;
        idx_t last_digit = 0;
        bool ok = true;
        while (temp)
        {
            idx_t digit = temp % b;
            temp /= b;
            if (!first)
            {
                if (digit >= last_digit)
                {
                    ok = false;
                    break;
                }
            }
            last_digit = digit;
            first = false;
        }
        if (ok)
            ++count;
    }
    return count;
}
