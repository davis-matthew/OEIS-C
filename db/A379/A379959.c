/*
 * A379959  a(n) = 1 if sigma(phi(n)) < n, otherwise 0.
 * https://oeis.org/A379959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(phi(n)) < n, otherwise 0
 * Range:     u8 n <= 108, u16 n <= 9723, u32 n <= 9723, u64 n <= 9723,
 *            u128 n <= 9723, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) /* outside the OEIS offset; return 0 */
        return 0;

    seq_t phi_val = calc_phi(n);          /* φ(n) */
    idx_t phi_idx = (idx_t)phi_val;       /* φ(n) ≤ n, fits in idx_t */
    seq_t sigma_val = sigma_of(phi_idx);  /* σ(φ(n)) */

    return (sigma_val < (seq_t)n) ? 1 : 0;
}
