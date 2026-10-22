/*
 * A306412  a(n) = phi(n^8) = n^7*phi(n).
 * https://oeis.org/A306412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = phi(n^8) = n^7*phi(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t phi_n = calc_phi(sn);
    seq_t n_pow7 = pow_uint((uint64_t)n, (idx_t)7);
    return n_pow7 * phi_n;
}
