/*
 * A318463  a(n) = Sum_{d|n, d < n/d} (d AND n/d), where AND is bitwise-and
 *          (A004198).
 * https://oeis.org/A318463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{d|n, d < n/d} (d AND (n/d))
 * Range:     u8 n <= 2519, u16 n <= 9703, u32 n <= 9703, u64 n <= 9703,
 *            u128 n <= 9703, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A318463. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;
            if (d < e)
            {
                sum += ((seq_t)d & (seq_t)e);
            }
        }
    }
    return sum;
}
