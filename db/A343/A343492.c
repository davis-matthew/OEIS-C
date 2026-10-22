/*
 * A343492  a(n) = Sum_{k=1..n} 5^(gcd(k, n) - 1).
 * https://oeis.org/A343492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * 5^(d-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   phi, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/power5.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n=0, return 0 */
    seq_t total = (seq_t)0;
    idx_t d;
    for (d = (idx_t)1; d * d <= n; ++d) {
        if (n % d == (idx_t)0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            /* term for divisor d1 */
            seq_t phi1 = phi(d2);                       /* phi(n/d1) */
            seq_t pow1 = power5(d1 - (idx_t)1);         /* 5^(d1-1) */
            total += phi1 * pow1;
            /* if d1 != d2 add the paired divisor */
            if (d1 != d2) {
                seq_t phi2 = phi(d1);                   /* phi(n/d2) */
                seq_t pow2 = power5(d2 - (idx_t)1);     /* 5^(d2-1) */
                total += phi2 * pow2;
            }
        }
    }
    return total;
}
