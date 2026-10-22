/*
 * A311843  Coordination sequence Gal.4.38.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 4n + 2*floor(n/8) + e(n) * where
 *            e(n) = 0 if n mod 8 ∈ {0,1,2}, * 1 if n mod 8 ∈ {3,4,5}, * 2 if
 *            n mod 8 ∈ {6,7}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* precomputed extra values for n mod 8 */
    static const uint8_t extra[8] = {0, 0, 0, 1, 1, 1, 2, 2};

    idx_t t = n / 8;          /* floor(n/8) */
    uint8_t e = extra[n & 7]; /* n % 8 */

    /* compute 4n + 2*floor(n/8) + e safely in seq_t */
    seq_t result = ((seq_t)n << 2);       /* 4 * n */
    result += (seq_t)(t * 2);             /* 2 * floor(n/8) */
    result += (seq_t)e;                   /* extra term */

    return result;
}
