/*
 * A373336  Expansion of Sum_{k>=1} x^k / (1 + x^k + x^(2*k) + x^(3*k) +
 *          x^(4*k) + x^(5*k) + x^(6*k)).
 * https://oeis.org/A373336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{d|n} g(d), where g(d) = 1 if d≡1 (mod 7), g(d) = –1 if
 *            d≡2 (mod 7), and g(d) = 0 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d <= n / d; ++d) {   /* iterate over divisors up to sqrt(n) */
        if (n % d != 0)
            continue;
        idx_t d2 = n / d;                   /* complementary divisor */
        /* Add contribution of divisor d */
        if (d % 7 == 1)
            result += (seq_t)1;
        else if (d % 7 == 2)
            result -= (seq_t)1;

        /* If distinct, add contribution of complementary divisor d2 */
        if (d2 != d) {
            if (d2 % 7 == 1)
                result += (seq_t)1;
            else if (d2 % 7 == 2)
                result -= (seq_t)1;
        }
    }
    return result;
}
