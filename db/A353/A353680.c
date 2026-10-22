/*
 * A353680  a(n) = 1 if n is odd and phi(sigma(n)) is equal to phi(n),
 *          otherwise 0.
 * https://oeis.org/A353680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and phi(sigma(n)) == phi(n), otherwise 0
 * Range:     u8 n <= 156, u16 n <= 9383, u32 n <= 9383, u64 n <= 9383,
 *            u128 n <= 9383, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A353680 sequence term for a given n.
   Uses utility functions sigma_of and calc_phi provided in the build. */
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* Defensive: n should be >= 1, but we handle n == 0 gracefully. */
    if (n == 0)
        return 0;

    /* Return 0 immediately if n is even. */
    if ((n & 1) == 0)
        return 0;

    /* Compute sigma(n) and compare phi(sigma(n)) with phi(n). */
    seq_t sigma_n = sigma_of(n);        /* sigma(n) as seq_t */
    seq_t phi_sigma = calc_phi(sigma_n); /* phi(sigma(n)) */
    seq_t phi_n = calc_phi((seq_t)n);    /* phi(n) */

    return (phi_sigma == phi_n) ? 1 : 0;
}
