/*
 * A353637  a(n) = 1 if phi(sigma(n)) is equal to phi(n), otherwise 0.
 * https://oeis.org/A353637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff φ(σ(n)) == φ(n), otherwise 0
 * Range:     u8 n <= 9872, u16 n <= 9873, u32 n <= 9873, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   calc_phi, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the required utility functions */
#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* Compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* Compute φ(σ(n)) and φ(n) */
    seq_t phi_sigma = calc_phi(sigma);
    seq_t phi_n = phi(n);

    /* Return 1 if the two values are equal, 0 otherwise */
    return (phi_sigma == phi_n) ? (seq_t)1 : (seq_t)0;
}
