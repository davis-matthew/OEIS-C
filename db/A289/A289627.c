/*
 * A289627  a(n) = Sum_{d|n} mu(phi(d))*n/d.
 * https://oeis.org/A289627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(phi(d)) * n/d
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, mobius, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any positive n.  The implementation is fast enough for
   n up to ~10⁴ and works for arbitrary-precision seq_t and 64‑bit idx_t. */
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/mobius.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for 0, return 0 */
        return 0;

    seq_t sum = 0;
    idx_t limit = _floor_isqrt_u64(n);

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d != 0) continue;

        /* divisor d */
        idx_t div1 = d;
        idx_t quot1 = n / div1;
        seq_t phi1 = phi(div1);            /* phi(div1) fits in idx_t */
        seq_t mu1  = mobius((idx_t)phi1);  /* mu(phi(d)) */
        sum += mu1 * (seq_t)quot1;

        /* paired divisor n/d if different */
        idx_t div2 = n / d;
        if (div2 != div1) {
            idx_t quot2 = n / div2;          /* == d */
            seq_t phi2 = phi(div2);
            seq_t mu2  = mobius((idx_t)phi2);
            sum += mu2 * (seq_t)quot2;
        }
    }

    return sum;
}
