/*
 * A290255  Number of 0's following directly the first 1 in the binary
 *          representation of n.
 * https://oeis.org/A290255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = number of 0's that immediately follow the most
 *            significant 1 in the binary expansion of n; recurrence: a(2^k) =
 *            k, otherwise a(n) = a(n / 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern idx_t floor_log2_uint64(idx_t v); /* Provided by the build – not used */

/* Find the index (0‑based) of the highest set bit in x. */
static inline idx_t msb_index(idx_t x)
{
    idx_t r = 0;
    while (x > 1)
    {
        x >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification. */
    idx_t msb = msb_index(n);
    idx_t high = ((idx_t)1 << msb);
    idx_t rem  = n - high;

    if (rem == 0)                      /* n is a power of two */
    {
        return (seq_t)msb;
    }
    else
    {
        idx_t msb2 = msb_index(rem);
        idx_t zeros = msb - msb2 - 1;
        return (seq_t)zeros;
    }
}
