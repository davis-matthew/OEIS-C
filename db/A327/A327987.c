/*
 * A327987  a(n) = Sum_{d|n} d & (n/d), where & is the bitwise AND operator,
 *          with a(0) = 0.
 * https://oeis.org/A327987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d & (n/d)
 * Range:     u8 n <= 1259, u16 n <= 9456, u32 n <= 9456, u64 n <= 9456,
 *            u128 n <= 9456, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    /* find floor(sqrt(n)) */
    idx_t lim = 0;
    for (idx_t i = 1; (i * i) <= n; ++i)
        lim = i;

    for (idx_t d = 1; d <= lim; ++d)
    {
        if (n % d == 0)
        {
            idx_t q = n / d;
            seq_t val = ((seq_t)d) & ((seq_t)q);
            if (d == q)
                sum += val;          /* perfect square divisor */
            else
            {
                sum += val;          /* divisor d */
                sum += val;          /* divisor q (same value) */
            }
        }
    }
    return sum;
}
