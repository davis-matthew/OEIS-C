/*
 * A274136  a(n) = (n+1)*(2*n+2)!/(n+2).
 * https://oeis.org/A274136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(2*n+2)!/(n+2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute result = (n+1) * Prod_{i=1}^{2n+2, i≠n+2} i  */
    seq_t result = (seq_t)(n + 1);
    idx_t upper = 2 * n + 2;
    idx_t skip = n + 2;
    for (idx_t i = 1; i <= upper; ++i)
    {
        if (i == skip)
            continue;    /* omit the factor (n+2) */
        result = result * (seq_t)i;
    }
    return result;
}
