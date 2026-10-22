/*
 * A319210  a(n) = phi(n^2 - 1)/2 where phi is A000010.
 * https://oeis.org/A319210
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n^2 - 1)/2
 * Range:     u8 n <= 16, u16 n <= 258, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* n >= 2 */
    seq_t m = (seq_t)n * (seq_t)n - 1;
    seq_t ph = calc_phi(m);
    return ph / 2;
}
