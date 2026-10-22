/*
 * A313833  Coordination sequence Gal.6.341.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((31*n + 5)/6) - s(n) where s(n) = 1 if n mod 12 ∈
 *            {1,2,4,5,9}, otherwise 0; a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute floor((31*n + 5)/6) */
    seq_t base = ((seq_t)n * 31 + 5) / 6;

    /* Subtract 1 for the special residues */
    idx_t r = n % 12;
    if (r == 1 || r == 2 || r == 4 || r == 5 || r == 9) {
        base -= 1;
    }

    return base;
}
