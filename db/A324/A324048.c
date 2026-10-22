/*
 * A324048  a(n) = A000203(n) - A083254(n) = n + sigma(n) - 2*phi(n).
 * https://oeis.org/A324048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + sigma(n) - 2*phi(n)
 * Range:     u8 n <= 83, u16 n <= 8609, u32 n <= 8609, u64 n <= 8609,
 *            u128 n <= 8609, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t sigma = sigma_of(n);    /* σ(n) */
    seq_t phi_val = phi(n);       /* φ(n) */

    return n_val + sigma - (phi_val << 1);
}
