/*
 * A327202  a(n) = [(2n+2)r] - [(n+2)r] - [nr], where [ ] = floor and r =
 *          (1+sqrt(5))/2.
 * https://oeis.org/A327202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(2n+2)φ⌋ – ⌊(n+2)φ⌋ – ⌊nφ⌋, φ = (1+√5)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_phi.h"

seq_t a(idx_t n)
{
    idx_t n2   = n + n + 2;   /* 2n + 2  */
    idx_t nplus2 = n + 2;     /* n + 2   */

    seq_t t1 = floor_phi(n2);
    seq_t t2 = floor_phi(nplus2);
    seq_t t3 = floor_phi(n);

    return t1 - t2 - t3;
}
