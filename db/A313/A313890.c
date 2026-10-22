/*
 * A313890  Coordination sequence Gal.6.648.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n≥1 let q = n / 3, r = n % 3. If r = 0 a(n) = 16*q.
 *            If r = 1 a(n) = 16*q + (q % 6 == 5 ? 6 : 5). If r = 2 a(n) =
 *            16*q + (q % 6 == 0 ? 10 : 11)
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t q = n / 3;
    int r = n % 3;

    seq_t base = (seq_t)q * 16;
    if (r == 0)
        return base;
    else if (r == 1)
    {
        seq_t offset = (q % 6 == 5) ? (seq_t)6 : (seq_t)5;
        return base + offset;
    }
    else /* r == 2 */
    {
        seq_t offset = (q % 6 == 0) ? (seq_t)10 : (seq_t)11;
        return base + offset;
    }
}
