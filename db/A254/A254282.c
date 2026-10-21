/*
 * A254282  Expansion of (1 - (1 - 27*x)^(1/3)) / (9*x).
 * https://oeis.org/A254282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+1)*a(n) = 9*(3*n-1)*a(n-1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;          /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute factor = 9*(3*i-1)  (fits in seq_t) */
        seq_t factor = (seq_t)9 *
                       (((seq_t)3 * (seq_t)i) - (seq_t)1);
        seq_t denom = (seq_t)(i + 1);

        /* Cancel common divisors to keep intermediate values small */
        seq_t g = gcd(factor, denom);
        factor /= g;
        denom  /= g;

        g = gcd(result, denom);
        result /= g;
        denom  /= g;

        g = gcd(factor, denom);
        factor /= g;
        denom  /= g;

        /* denom should now be 1 */
        result *= factor;
        if (denom != (seq_t)1) {
            result /= denom;
        }
    }

    return result;
}
