/*
 * A312903  Coordination sequence Gal.5.65.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n>=1 ? 3 : 0) + Σ_{k=2..n} p[(k-2) mod 10], where p
 *            = {5,4,4,5,5,4,4,5,4,4}. This yields the OEIS sequence A312903
 *            (Coordination sequence Gal.5.65.2)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t pattern[10] = {5, 4, 4, 5, 5, 4, 4, 5, 4, 4};

seq_t a(idx_t n)
{
    seq_t res = 1;          /* a(0) = 1 */
    if (n == 0) return res;

    res += 3;               /* a(1) = 1 + 3 = 4 */

    for (idx_t k = 2; k <= n; ++k)
    {
        idx_t idx = (k - 2) % 10;     /* index into pattern */
        res += pattern[idx];
    }

    return res;
}
