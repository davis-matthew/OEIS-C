/*
 * A269250  Number of times the digit 0 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 2480, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of times the digit 0 appears in the decimal expansion of n^3 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute n³ */
    seq_t x = (seq_t)n;          /* x = n */
    seq_t y = x * x;             /* y = n² */
    y *= (seq_t)n;               /* y = n³ */

    /* Count zeros in the decimal representation of y */
    uint64_t cnt = 0;
    const seq_t ten = (seq_t)10;
    while (y > (seq_t)0)
    {
        if ((y % ten) == (seq_t)0)
            ++cnt;
        y /= ten;
    }
    return (seq_t)cnt;
}
