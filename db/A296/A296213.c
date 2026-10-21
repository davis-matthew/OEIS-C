/*
 * A296213  a(n) = 1 if both 1+phi(k) and 1+sigma(k) are squares, 0 otherwise.
 * https://oeis.org/A296213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 7253, u32 n <= 9893, u64 n <= 9893,
 *            u128 n <= 9893, big any n
 * Helpers:   calc_phi, floor_sqrt, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Check whether 1+phi(n) and 1+sigma(n) are perfect squares. */
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to a seq_t for phi calculation */
    seq_t k = (seq_t)n;

    /* Euler totient and sum-of-divisors of n */
    seq_t phi   = calc_phi(k);
    seq_t sigma = sigma_of(n);

    /* Add 1 to each */
    seq_t phi1   = phi + 1;
    seq_t sigma1 = sigma + 1;

    /* Floor square roots */
    seq_t sqrt_phi   = floor_sqrt(phi1);
    seq_t sqrt_sigma = floor_sqrt(sigma1);

    /* Both must be perfect squares */
    if (sqrt_phi * sqrt_phi == phi1 && sqrt_sigma * sqrt_sigma == sigma1)
        return 1;
    else
        return 0;
}
