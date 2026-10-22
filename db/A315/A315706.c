/*
 * A315706  Coordination sequence Gal.5.294.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 n – ( 2·⌊n/9⌋ + [n mod 9 ≥ 3] + [n mod 9 ≥ 7] ), a(0) =
 *            1
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sn = (seq_t)n;                 /* n converted to seq_t */
    seq_t term = (seq_t)6 * sn;          /* 6*n */

    /* compute d(n) = 2*floor(n/9) + (r≥3) + (r≥7) */
    idx_t q = n / 9;
    idx_t r = n % 9;

    seq_t d = (seq_t)2 * (seq_t)q;       /* 2*floor(n/9) */
    if (r >= 3) d += (seq_t)1;
    if (r >= 7) d += (seq_t)1;

    return term - d;
}
