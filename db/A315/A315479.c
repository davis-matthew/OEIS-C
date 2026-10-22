/*
 * A315479  Coordination sequence Gal.6.210.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence starts with a(0)=1 and for n>=1 the increment
 *            a(n)-a(n-1) equals 5 or 6. The “extra 1” (giving a 6-step)
 *            occurs precisely when the index i (1‑based) satisfies i >= 5 and
 *            i mod 12 ∈ {0,1,5,6,7,8}. Hence a(n) = 1 + Σ_{i=1..n} (5 + δ_i)
 *            , where δ_i = 1 if the above condition holds, othe...
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;                     /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        res += 5;
        if (i >= 5) {
            idx_t r = i % 12;          /* remainder 0..11 */
            if (r == 0 || r == 1 || r == 5 ||
                r == 6 || r == 7 || r == 8)
                res += 1;              /* step of 6 instead of 5 */
        }
    }
    return res;
}
