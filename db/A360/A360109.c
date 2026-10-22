/*
 * A360109  a(n) = 1 if n is not multiple of 4, but its arithmetic derivative
 *          is, otherwise 0.
 * https://oeis.org/A360109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is not a multiple of 4 and its arithmetic
 *            derivative is a multiple of 4, otherwise 0
 * Range:     u8 n <= 9084, u16 n <= 9084, u32 n <= 9084, u64 n <= 9084,
 *            u128 n <= 9084, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t used in loops */

static seq_t arithmetic_derivative(idx_t n)
{
    if (n < 2) return 0;          /* 0' = 0, 1' = 0 */
    idx_t orig = n;               /* keep copy for division */
    idx_t m = n;
    seq_t deriv = 0;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* term = e * (n / p) */
            deriv += (seq_t)e * (seq_t)(orig / p);
        }
    }

    if (m > 1) { /* remaining prime factor with exponent 1 */
        deriv += (seq_t)1 * (seq_t)(orig / m);
    }

    return deriv;
}

seq_t a(idx_t n)
{
    if (n % 4 == 0) return 0; /* definition requires n not multiple of 4 */

    seq_t d = arithmetic_derivative(n);
    return ((d % (seq_t)4) == 0) ? 1 : 0;
}
