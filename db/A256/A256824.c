/*
 * A256824  Reverse concatenation of distinct digits of all divisors of n in
 *          base 10.
 * https://oeis.org/A256824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   reverse concatenation of distinct decimal digits of all *
 *            divisors of n, sorted in decreasing order (OEIS A256824)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    // Convert index to a 64‑bit unsigned integer for looping
    uint64_t nn = (uint64_t)n;

    // Bitmask of digits that appear in any divisor of nn
    uint16_t mask = 0;

    // Enumerate all divisors by iterating through 1..nn
    for (uint64_t d = 1; d <= nn; ++d)
    {
        if (nn % d == 0)           // d is a divisor of nn
        {
            uint64_t t = d;
            // Extract all decimal digits of the divisor
            while (t > 0)
            {
                mask |= 1U << (t % 10);
                t /= 10;
            }
        }
    }

    // Build the final number by appending digits from 9 down to 0
    seq_t result = 0;
    for (int d = 9; d >= 0; --d)
    {
        if (mask & (1U << d))
            result = result * 10 + d;
    }
    return result;
}
