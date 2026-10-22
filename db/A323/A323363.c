/*
 * A323363  Dirichlet inverse of Dedekind's psi, A001615.
 * https://oeis.org/A323363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(p^e) = (-1)^e * (p+1); multiplicative over prime powers
 * Range:     i8 n <= 65, i16 n <= 9917, i32 n <= 9917, i64 n <= 9917,
 *            i128 n <= 9917, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; n=0 is unused */
    if (n == 1) return 1;          /* a(1) = 1 */

    seq_t result = 1;
    idx_t rem = n;

    for (idx_t p = 2; p <= rem / p; ++p) {
        if (rem % p == 0) {
            unsigned exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            if (exp & 1U)            /* odd exponent → flip sign */
                result = -result;
            result = result * (seq_t)(p + 1);
        }
    }

    if (rem > 1) {                  /* remaining prime factor */
        result = -result;           /* exponent is 1 (odd) */
        result = result * (seq_t)(rem + 1);
    }

    return result;
}
