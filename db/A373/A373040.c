/*
 * A373040  a(n) = (A084218(n) - 1)/12.
 * https://oeis.org/A373040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A084218(n) - 1) / 12 where A084218(n) = ∑_{d|n} g(d) and
 *            g(p^k) = 1 (k = 0) p^(4k) - p^(4k-2) (k ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 477, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   local_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/local_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;                /* a(1) = 0 */

    seq_t A = 1;                         /* product of local factors */
    idx_t tmp = n;

    for (idx_t p = 2; (idx_t)p * (idx_t)p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            A *= local_factor(p, e);
        }
    }
    /* remaining prime factor, if any */
    if (tmp > 1) {
        A *= local_factor(tmp, 1);
    }

    /* A now equals A084218(n). Compute a(n). */
    return (A - 1) / (seq_t)12;
}
