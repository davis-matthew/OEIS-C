/*
 * A312894  Coordination sequence Gal.4.38.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + (#{k | 2 ≤ k ≤ n & k ≡ 2 or 7 (mod 8)})
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) = 1 */

    /* Count indices k ∈ [2,n] with k ≡ 2 or 7 (mod 8) */
    idx_t c2 = 0, c7 = 0;
    if (n >= 2)            /* k == 2 mod 8 */
        c2 = (n - 2) / 8 + 1;
    if (n >= 7)            /* k == 7 mod 8 */
        c7 = (n - 7) / 8 + 1;

    seq_t count = (seq_t)c2 + (seq_t)c7;

    /* a(n) = 4*n + count */
    return (seq_t)4 * (seq_t)n + count;
}
