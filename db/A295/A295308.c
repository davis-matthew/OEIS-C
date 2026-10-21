/*
 * A295308  Characteristic function for A066694: a(n) = 1 if n <
 *          phi(sigma(n)), 0 otherwise.
 * https://oeis.org/A295308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n < phi(sigma(n)), 0 otherwise
 * Range:     u8 n <= 4910, u16 n <= 8968, u32 n <= 8968, u64 n <= 8968,
 *            u128 n <= 8968, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t sigma   = sigma_of(n);
    seq_t phi_sig = calc_phi(sigma);
    return ((seq_t)n < phi_sig) ? (seq_t)1 : (seq_t)0;
}
