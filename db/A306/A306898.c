/*
 * A306898  a(n) = Sum_{d|n} 2^d*phi(2*n/d).
 * https://oeis.org/A306898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^d * phi(2 * n / d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 3118 (2026-11-01)
 * Helpers:   ipow_u64_idx, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* term for d1 */
            idx_t m1 = 2 * (n / d1);          /* 2 * (n / d1) */
            seq_t term1 = ipow_u64_idx(2, d1) * totient(m1);
            sum += term1;

            /* term for d2 if distinct */
            if (d1 != d2) {
                idx_t m2 = 2 * (n / d2);
                seq_t term2 = ipow_u64_idx(2, d2) * totient(m2);
                sum += term2;
            }
        }
    }
    return sum;
}
