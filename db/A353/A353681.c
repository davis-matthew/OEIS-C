/*
 * A353681  a(n) = 1 if phi(sigma(n)) > phi(n), otherwise 0.
 * https://oeis.org/A353681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [phi(sigma(n)) > phi(n)]
 * Range:     u8 n <= 107, u16 n <= 9952, u32 n <= 9952, u64 n <= 9952,
 *            u128 n <= 9952, big any n
 * Helpers:   calc_phi, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"
#include "../utils/phi.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t sigma = sigma_of(n);
    seq_t phi_sigma = calc_phi(sigma);
    seq_t phi_n = phi(n);
    return (phi_sigma > phi_n) ? 1 : 0;
}
