/*
 * A314144  Coordination sequence Gal.6.634.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + floor(n/3) + c(n) where c(n) = 1 if n == 0 or n%6
 *            == 5 or (n%6 == 2 or 4) && ((n/6)%2 == 0); otherwise c(n) = 0
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = n;                  /* x = n in seq_t */
    seq_t res = 5 * x + x / 3;    /* base part: 5n + floor(n/3) */
    idx_t r = n % 6;
    seq_t c = 0;
    if (r == 5 || ((r == 2 || r == 4) && ((n / 6) % 2 == 0)) || (n == 0))
        c = 1;
    return res + c;
}
