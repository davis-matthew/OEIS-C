/*
 * A292608  a(n) = 2*n + 1 + valuation(n, 2) with valuation(n, 2) =
 *          A007814(n).
 * https://oeis.org/A292608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + 1 + valuation(n, 2)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is irrelevant */

    seq_t v = 0;                   /* value of v2(n) */
    idx_t m = n;
    while ((m & 1) == 0)           /* count trailing zeros in binary representation */
    {
        m >>= 1;
        v++;
    }

    /* compute 2*n + 1 + v */
    seq_t result = (seq_t)n * ((seq_t)2) + (seq_t)1 + v;
    return result;
}
