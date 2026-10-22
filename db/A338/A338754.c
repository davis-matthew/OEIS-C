/*
 * A338754  Duplicate each decimal digit of n, so 0 -> 00, ..., 9 -> 99.
 * https://oeis.org/A338754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 11 * A051022(n) where A051022(n) = ∑ d[i] * 100^i (d[i]
 *            are decimal digits of n)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t res   = 0; /* Accumulator for ∑ d[i] * 100^i */
    seq_t mult  = 1; /* Current power of 100 (100^i) */
    idx_t tmp   = n;
    while (tmp > 0)
    {
        /* Least significant decimal digit */
        unsigned d = (unsigned)(tmp % 10);
        res += (seq_t)d * mult;
        mult *= 100;   /* Next power of 100 */
        tmp /= 10;
    }
    /* Duplicate each decimal digit: multiply by 11 */
    res *= 11;
    return res;
}
