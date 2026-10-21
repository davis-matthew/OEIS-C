/*
 * A291503  a(n) is the smallest k such that sigma(k) = phi(n), or 0 if no
 *          such k exists.
 * https://oeis.org/A291503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k such that sigma(k) = phi(n); returns 0 if none
 * Range:     u8 n <= 282, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t phi_n = phi(n);
    for (idx_t k = 1; k <= n; ++k) {
        if (sigma_of(k) == phi_n)
            return k;
    }
    return 0;
}
