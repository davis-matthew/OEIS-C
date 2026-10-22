/*
 * A351116  Sum of the balanced numbers <= n.
 * https://oeis.org/A351116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k<=n, phi(k)|sigma(k)} k
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t phi   = calc_phi((seq_t)k);
        seq_t sigma = sigma_of(k);
        /* phi(k) is always > 0 */
        if (sigma % phi == 0) {
            sum += (seq_t)k;
        }
    }
    return sum;
}
