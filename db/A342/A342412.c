/*
 * A342412  a(n) = Sum_{k=1..n} (n/gcd(k,n))^(n-2).
 * https://oeis.org/A342412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * d^(n-2) (OEIS A342412)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 1, because the exponent would be -1. */
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* contribution of divisor d */
            seq_t term = phi(d) * big_pow((seq_t)d, (seq_t)(n - 2));
            sum += term;

            /* complementary divisor n/d, skip if it's the same */
            idx_t e = n / d;
            if (e != d) {
                seq_t term2 = phi(e) * big_pow((seq_t)e, (seq_t)(n - 2));
                sum += term2;
            }
        }
    }

    return sum;
}
