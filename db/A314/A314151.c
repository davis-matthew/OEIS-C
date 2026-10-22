/*
 * A314151  Coordination sequence Gal.6.621.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(11*n/2) + (n mod 12 in {7,9,11} ? 1 : 0) * with
 *            a(0) = 1. This reproduces the coordination sequence *
 *            Gal.6.621.3
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* offset term */
    seq_t sn = (seq_t)n;
    seq_t val = sn * (seq_t)11 / (seq_t)2; /* floor division */

    idx_t m = n % (idx_t)12;
    if (m == 7 || m == 9 || m == 11) {
        val += (seq_t)1;
    }
    return val;
}
