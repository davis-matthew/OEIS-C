/*
 * A315123  Coordination sequence Gal.6.45.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Piecewise linear recurrence derived from the pattern for n >= 8:
 *            let k = n/8, r = n%8 a(8k) = 36k a(8k+1) = 36k+6 a(8k+2) =
 *            36k+12 (except n==2) a(8k+3) = 32k+12 a(8k+4) = 32k+16 a(8k+5) =
 *            32k+18 a(8k+6) = 32k+22 a(8k+7) = 32k+24 Special cases: a(0)=1,
 *            a(2)=10
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
        return 1;
    if (n == 2)
        return 10;
    idx_t k = n / 8;
    idx_t r = n % 8;
    switch (r)
    {
        case 0:
            return ((seq_t)36) * ((seq_t)k);
        case 1:
            return ((seq_t)36) * ((seq_t)k) + (seq_t)6;
        case 2:
            return ((seq_t)36) * ((seq_t)k) + (seq_t)12;
        case 3:
            return ((seq_t)32) * ((seq_t)k) + (seq_t)12;
        case 4:
            return ((seq_t)32) * ((seq_t)k) + (seq_t)16;
        case 5:
            return ((seq_t)32) * ((seq_t)k) + (seq_t)18;
        case 6:
            return ((seq_t)32) * ((seq_t)k) + (seq_t)22;
        case 7:
            return ((seq_t)32) * ((seq_t)k) + (seq_t)24;
        default:
            return 0;  /* unreachable */
    }
}
