/*
 * A340375  a(n) = 1 if n is of the form 2^i - 2^j with i >= j, and 0
 *          otherwise.
 * https://oeis.org/A340375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃ i ≥ j such that n = 2^i – 2^j, otherwise 0
 * Range:     u8 n <= 9580, u16 n <= 9580, u32 n <= 9580, u64 n <= 9580,
 *            u128 n <= 9580, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_power_of_two(idx_t x)
{
    return x && ((x & (x - 1)) == 0);
}

seq_t a(idx_t n)
{
    /* All required values are ≤ 64‑bit, so we only need to test
       values of 2^i for i = 0…63.  The loop stops when shifting
       would overflow. */
    idx_t val = 1;
    for (idx_t i = 0; i < 64; ++i)
    {
        if (val > n)
        {
            idx_t diff = val - n;               /* 2^i – n = 2^j */
            if (is_power_of_two(diff))
                return (seq_t)1;
        }
        if (i == 63)
            break;                               /* avoid undefined shift */
        val <<= 1;
    }
    return (seq_t)0;
}
