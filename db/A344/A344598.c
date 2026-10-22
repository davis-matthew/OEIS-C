/*
 * A344598  a(n) = Sum_{k=1..n} phi(k) * (floor(n/k)^2 - floor((n-1)/k)^2).
 * https://oeis.org/A344598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} phi(d)*(2*n/d - 1)
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t result = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            seq_t phi1 = phi(d1);
            seq_t term1 = phi1 * ( (seq_t)(2 * (n / d1) - 1) );
            result += term1;

            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t phi2 = phi(d2);
                seq_t term2 = phi2 * ( (seq_t)(2 * (n / d2) - 1) );
                result += term2;
            }
        }
    }
    return result;
}
