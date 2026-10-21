/*
 * A279635  Denominator of (0 followed by A005126(n)= 2, 4, 7, ...)/2^n, a
 *          sequence corresponding to A271573.
 * https://oeis.org/A279635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(n - valuation(2^(n-1)+n,2))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static inline idx_t v2_uint64(idx_t n)
{
    idx_t v = 0;
    while ((n & 1ULL) == 0ULL)
    {
        ++v;
        n >>= 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    idx_t v = v2_uint64(n);          /* v2(n) */
    idx_t v2s = v;                   /* v2(2^(n-1)+n) */
    if (n == 1 || n == 2)
        v2s = v + 1;                 /* extra carry for n=1,2 */

    idx_t exp = n - v2s;             /* exponent for 2 */
    return pow2(exp);
}
