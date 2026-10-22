/*
 * A304653  a(n) = (-1)^Omega(n) if n is not a perfect power > 1, and 0
 *          otherwise.
 * https://oeis.org/A304653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^Omega(n) if n is not a perfect power > 1; 0
 *            otherwise
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   int_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* index starts at 1, but safe fallback */
    if (n == 1) return (seq_t)1;   /* Omega(1)=0, not a perfect power >1 */

    uint64_t N = (uint64_t)n;
    uint64_t k = N;
    int omega = 0;
    int g = 0;                     /* gcd of exponents */

    for (uint64_t p = 2; p <= k / p; ++p) {
        if (k % p == 0) {
            int exp = 0;
            while (k % p == 0) {
                k /= p;
                ++exp;
                ++omega;
            }
            if (g == 0) g = exp;
            else g = int_gcd(g, exp);
        }
    }

    if (k > 1) {                   /* remaining prime factor */
        ++omega;
        if (g == 0) g = 1;
        else g = int_gcd(g, 1);
    }

    if (g > 1) return (seq_t)0;    /* perfect power > 1 */

    return (seq_t)((omega & 1) ? -1 : 1);
}
