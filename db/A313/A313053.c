/*
 * A313053  Coordination sequence Gal.5.114.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (23*n + f(n))/5 where f(n) = 5 if n == 0 f(n) = 0 if n%5
 *            == 0 and n > 0 f(n) = 2 * ((n/5) % 2) if n%5 == 1 f(n) = 0 if
 *            n%5 == 2 f(n) = 1 if n%5 == 3 f(n) = 3 * ((n/5) % 2) if n%5 == 4
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   _f_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_f_n.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t f  = (seq_t)_f_n(n);
    seq_t val = nn * 23 + f;
    return val / 5;
}
