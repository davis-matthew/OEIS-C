/*
 * A269130  a(n) = n + (n base 2 regarded as a decimal number).
 * https://oeis.org/A269130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n + (n base 2 regarded as a decimal number)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t t = n;
    int msb = 0;
    while ((t >>= 1) != 0)
        msb++;
    for (int i = msb; i >= 0; --i)
    {
        uint64_t bit = (n >> i) & 1;
        val = val * 10 + (seq_t)bit;
    }
    return val + (seq_t)n;
}
