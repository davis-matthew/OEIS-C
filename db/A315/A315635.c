/*
 * A315635  Coordination sequence Gal.5.138.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6; for n>=2 a(n) = a(n-1) + 4 + 2·[(floor(n/3) mod
 *            3 == 0) OR (n mod 3 == 2)]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)6;

    seq_t val = (seq_t)6;
    for (idx_t k = 2u; k <= n; ++k) {
        idx_t q = k / 3u;
        bool flag = (q % 3u == 0u) || (k % 3u == 2u);
        val += flag ? (seq_t)6 : (seq_t)4;
    }
    return val;
}
