/*
 * A315463  Coordination sequence Gal.5.113.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence Gal.5.113.4 (A315463)
   Base terms for n = 0..4 are 1, 6, 11, 16, 21.
   For n ≥ 5 the increments repeat with period 9:
        if n % 9 == 5   -> increment = 4
        else if n % 9 == 0 or n % 9 == 1   -> increment = 6
        else                               -> increment = 5
   Thus  a(n) = a(n-1) + increment   for n ≥ 5.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first five terms directly */
    if (n < 5) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)6;
            case 2: return (seq_t)11;
            case 3: return (seq_t)16;
            case 4: return (seq_t)21;
        }
    }

    seq_t val = (seq_t)21;          /* a(4) */
    for (idx_t i = 5; i <= n; ++i) {   /* compute forward to a(n) */
        idx_t r = i % 9;
        seq_t inc;
        if (r == 5)
            inc = (seq_t)4;
        else if (r == 0 || r == 1)
            inc = (seq_t)6;
        else
            inc = (seq_t)5;
        val += inc;
    }
    return val;
}
