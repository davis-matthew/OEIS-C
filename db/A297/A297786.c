/*
 * A297786  Decimal expansion of 10980011/999^3.
 * https://oeis.org/A297786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the nth digit after the decimal point of 10980011/999^3
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work with 64–bit integers to avoid overflow when computing
       remainder and divisor.  The final digit (0–9) fits in any
       seq_t type. */
    const uint64_t NUM = 10980011ULL;
    const uint64_t DEN = 997002999ULL;   /* 999^3 */

    uint64_t r = NUM % DEN;  /* initial remainder */
    uint64_t digit = 0;

    /* Compute the digits up to position n */
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= 10ULL;
        digit = r / DEN;      /* current digit (0–9) */
        r     %= DEN;         /* new remainder for next digit */
    }

    return (seq_t)digit;
}
