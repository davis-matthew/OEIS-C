/*
 * A297780  Number of distinct runs in base-12 digits of n.
 * https://oeis.org/A297780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct digits of n in base 12
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle the trivial case for consistency */
    if (n == 0)
        return (seq_t)0;

    /* mask the 12 possible digits */
    unsigned int mask = 0;
    idx_t x = n;
    while (x > 0) {
        unsigned digit = x % 12;   /* 0 … 11 */
        mask |= 1U << digit;
        x /= 12;
    }

    /* count set bits in mask – at most 12 */
    seq_t count = 0;
    while (mask) {
        count += (seq_t)(mask & 1U);
        mask >>= 1;
    }
    return count;
}
