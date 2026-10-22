/*
 * A333169  a(n) = phi(n^2 + 1), where phi is the Euler totient function
 *          (A000010).
 * https://oeis.org/A333169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n^2 + 1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * ((seq_t)n) + (seq_t)1;
    return calc_phi(m);
}
