/*
 * A353682  a(n) = 1 if phi(sigma(n)) >= phi(n), otherwise 0.
 * https://oeis.org/A353682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if phi(sigma(n)) >= phi(n), otherwise 0
 * Range:     u8 n <= 107, u16 n <= 9974, u32 n <= 9974, u64 n <= 9974,
 *            u128 n <= 9974, big any n
 * Helpers:   calc_phi, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);          /* σ(n) */
    seq_t phisigma = calc_phi(sigma);  /* φ(σ(n)) */
    seq_t phin = phi(n);               /* φ(n)   */
    return (phisigma >= phin) ? (seq_t)1 : (seq_t)0;
}
