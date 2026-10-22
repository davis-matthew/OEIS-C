/*
 * A374134  a(n) = 1 if 2*phi(n) > n, otherwise 0, where phi is Euler's
 *          totient function phi, A000010.
 * https://oeis.org/A374134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if 2*phi(n) > n else 0
 * Range:     u8 n <= 8484, u16 n <= 8484, u32 n <= 8484, u64 n <= 8484,
 *            u128 n <= 8484, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t phi_val = phi(n);
    seq_t half = ((seq_t)n) >> 1;   // floor(n/2)
    return (phi_val > half) ? 1 : 0;
}
