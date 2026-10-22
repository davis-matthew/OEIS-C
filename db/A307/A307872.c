/*
 * A307872  Sum of the smallest parts in the partitions of n into 3 parts.
 * https://oeis.org/A307872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..⌊n/3⌋} k * (⌊(n-k)/2⌋ - k + 1)
 * Range:     u8 n <= 28, u16 n <= 190, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the smallest parts in the partitions of n into 3 parts. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* maximal k: floor(n/3) */
    idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 1; k <= maxk; ++k)
    {
        /* floor((n-k)/2) */
        seq_t half = (seq_t)(n - k) / 2;

        /* count of middle part values */
        seq_t cnt = half - (seq_t)k + 1;

        /* add contribution of minimal part k */
        sum += (seq_t)k * cnt;
    }

    return sum;
}
