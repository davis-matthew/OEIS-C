/*
 * A326121  Expansion of Sum_{k>=1} k^2 * x^(2*k) / (1 - k * x^k).
 * https://oeis.org/A326121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} d^(n/d), with a(1)=0
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 59, u64 n <= 122,
 *            u128 n <= 242, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently a(n) = Sum_{d|n, d>1} (n/d)^d) */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* a(1) = 0 */

    seq_t result = (seq_t)1;        /* term for divisor d = 1 : 1^(n) = 1 */

    /* Enumerate divisors d >= 2 up to sqrt(n) */
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            /* d is a proper divisor (< n) */
            seq_t term1 = big_pow((seq_t)d, (seq_t)(n / d));
            result += term1;

            idx_t e = n / d;                 /* complementary divisor */
            if (e != d && e < n) {           /* e is also proper and not equal to d */
                seq_t term2 = big_pow((seq_t)e, (seq_t)(n / e));
                result += term2;
            }
        }
    }

    return result;
}
