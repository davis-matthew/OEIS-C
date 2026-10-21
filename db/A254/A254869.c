/*
 * A254869  Seventh partial sums of cubes (A000578).
 * https://oeis.org/A254869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(n+6)*(n+7)*(n^2+7*n+7) /
 *            604800 (sequence A254869 – Seventh partial sums of cubes)
 * Range:     u8 none, u16 none, u32 n <= 31, u64 n <= 316, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Since the offset is 1, a(1) must be returned for n==1.
       For n==0 we return 0 (undefined behaviour is not required). */
    if (n == 0)
        return 0;

    /* Product of the first eight factors, divided by the denominator as we go. */
    seq_t result = (seq_t)1;            /* running product */
    seq_t denom  = 604800;              /* 2^7 * 3^3 * 5^2 * 7 */

    for (idx_t i = 0; i < 8; ++i) {
        seq_t f = (seq_t)n + (seq_t)i;

        /* Cancel any common divisor between the current factor and the
           remaining denominator to keep intermediate values small. */
        seq_t g = gcd(denom, f);
        if (g != 1) {
            denom /= g;
            f /= g;
        }

        result *= f;
    }

    /* Final factor: n^2 + 7*n + 7 */
    seq_t f = (seq_t)n * (seq_t)n + (seq_t)7 * (seq_t)n + (seq_t)7;
    seq_t g = gcd(denom, f);
    if (g != 1) {
        denom /= g;
        f /= g;
    }
    result *= f;

    /* After all cancellations the denominator should be 1,
       but we keep the division for safety. */
    return result / denom;
}
