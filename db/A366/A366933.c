/*
 * A366933  Expansion of Sum_{k>=1} k^4 * x^k/(1 - x^k)^4.
 * https://oeis.org/A366933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d^4 * binomial(n/d+2,3)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   divisor_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_term.h"

seq_t a(idx_t n) {
    seq_t res = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            res += divisor_term(n, d);
            idx_t e = n / d;
            if (e != d) {
                res += divisor_term(n, e);
            }
        }
    }
    return res;
}
