/*
 * A374045  a(n) = 1 if A328845(n) is even, otherwise 0, where A328845 is the
 *          first Fibonacci based variant of arithmetic derivative.
 * https://oeis.org/A374045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A328845(n) is even, else 0 (parity of the first
 *            Fibonacci based derivative)
 * Range:     u8 n <= 9515, u16 n <= 9515, u32 n <= 9515, u64 n <= 9515,
 *            u128 n <= 9515, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == (idx_t)0 || n == (idx_t)1) return (seq_t)1;   /* derivative is 0 (even) */

    /* Separate the factor 2 */
    idx_t m = n;
    idx_t e2 = 0;
    while ((m & (idx_t)1) == (idx_t)0) {   /* while m even */
        e2++;
        m >>= (idx_t)1;
    }

    /* If 2 appears with exponent >= 2, derivative is even. */
    if (e2 >= (idx_t)2) return (seq_t)1;

    /* If 2 appears exactly once, derivative is odd. */
    if (e2 == (idx_t)1) return (seq_t)0;

    /* Now m is odd, factorization of an odd integer. */
    bool parity = false;   /* false → derivative even, true → odd */

    idx_t p = (idx_t)3;
    while (p * p <= m) {
        if (m % p == (idx_t)0) {
            idx_t exp = (idx_t)0;
            while (m % p == (idx_t)0) {
                m /= p;
                exp++;
            }
            /* Toggle parity iff exponent odd and Fibonacci of p is odd
               (F(p) is odd ⇔ p mod 3 ≠ 0). */
            if ((exp & (idx_t)1) && (p % (idx_t)3 != (idx_t)0))
                parity = !parity;
        }
        p += (idx_t)2;
    }

    /* Remaining prime factor (if any). */
    if (m > (idx_t)1) {
        idx_t exp = (idx_t)1;
        if ((exp & (idx_t)1) && (m % (idx_t)3 != (idx_t)0))
            parity = !parity;
    }

    return parity ? (seq_t)0 : (seq_t)1;
}
