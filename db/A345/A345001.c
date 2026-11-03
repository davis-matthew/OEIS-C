/*
 * A345001  a(n) = sigma(n) + n' - 2n, where n' is the arithmetic derivative
 *          of n (A003415) and sigma is the sum of divisors (A000203).
 * https://oeis.org/A345001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sigma(n) + n' - 2*n
 * Range:     i8 n <= 47, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma is the sum of divisors (A000203) and n' is the arithmetic derivative (A003415). */

#include <stdint.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* the sequence starts at n = 1 */

    /* sum of divisors */
    seq_t sigma = sigma_of(n);

    /* arithmetic derivative via prime factorisation */
    seq_t deriv = 0;
    uint64_t temp   = (uint64_t)n;
    uint64_t original = temp;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            deriv += (seq_t)(original / p) * (seq_t)e;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        deriv += (seq_t)(original / temp) * (seq_t)1;
    }

    seq_t two_n = (seq_t)2 * (seq_t)n;
    return sigma + deriv - two_n;
}
