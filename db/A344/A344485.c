/*
 * A344485  a(n) = Sum_{d|n} (n-d) * phi(n/d).
 * https://oeis.org/A344485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n-d) * phi(n/d)
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* a(0) is undefined, but safe */

    seq_t result = 0;
    idx_t n_idx   = n;           /* keep original index for clarity */

    for (idx_t d = 1; d * d <= n_idx; ++d) {
        if (n_idx % d == 0) {
            idx_t d1 = d;          /* divisor d */
            idx_t d2 = n_idx / d;  /* complementary divisor n/d */

            /* contribution from divisor d1 */
            seq_t phi_d2 = phi(d2);                     /* phi(n/d) */
            result += ((seq_t)n_idx - (seq_t)d1) * phi_d2;

            /* contribution from complementary divisor d2, if distinct */
            if (d1 != d2) {
                seq_t phi_d1 = phi(d1);                     /* phi(d1) */
                result += ((seq_t)n_idx - (seq_t)d2) * phi_d1;
            }
        }
    }

    return result;
}
