/*
 * A344815  a(n) = Sum_{k=1..n} floor(n/k) * 4^(k-1).
 * https://oeis.org/A344815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (floor(n/k) * 4^(k-1))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow4   = 1;          /* 4^(k-1) for current k */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* floor(n/k) is n/k as integer division on idx_t */
        seq_t term = ((seq_t)(n / k)) * pow4;
        result += term;
        pow4 *= 4;             /* advance to next power of 4 */
    }

    return result;
}
