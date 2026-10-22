/*
 * A351231  Denominator of A003415(n) / A276086(n), where A003415 is the
 *          arithmetic derivative and A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A351231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, frac, look
 * Formula:   a(n) = A276086(n) / gcd(A003415(n), A276086(n)) where A276086 is
 *            the primorial base exp-function and A003415 is the arithmetic
 *            derivative
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 416, u64 n <= 2308,
 *            u128 n <= 9738, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* First 20 primes – sufficient for n up to 2^63 and for all
   relevant multibase representations. */
static const uint32_t primes[20] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u,
    23u, 29u, 31u, 37u, 41u, 43u, 47u,
    53u, 59u, 61u, 67u, 71u
};

/* Compute the primorial base exp-function A276086(n). */
static seq_t primorial_exp(idx_t n)
{
    if (n == 0u) return (seq_t)1;  /* 0 has digits all 0 → product = 1 */

    seq_t prod = (seq_t)1;
    idx_t temp = n;
    for (size_t i = 0; i < 20 && temp > 0u; ++i) {
        uint32_t p = primes[i];
        unsigned int digit = (unsigned int)(temp % p);
        if (digit != 0u) {
            seq_t mult = (seq_t)1;
            for (unsigned int k = 1u; k <= digit; ++k) mult *= (seq_t)p;
            prod *= mult;
        }
        temp /= p;  /* prepare for next digit */
    }
    return prod;
}

/* Compute the arithmetic derivative A003415(n). */
static seq_t arithmetic_derivative(idx_t n)
{
    if (n < 2u) return (seq_t)0;  /* derivative of 0 and 1 is 0 */

    seq_t deriv = (seq_t)0;
    idx_t temp = n;
    const idx_t orig = n;   /* original value for numerator */

    for (size_t i = 0; i < 20 && temp > 1u; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0u) {
            unsigned int exp = 0u;
            while (temp % p == 0u) {
                temp /= p;
                ++exp;
            }
            deriv += (seq_t)(orig / p) * (seq_t)exp;
        }
    }
    if (temp > 1u) {  /* remaining prime factor */
        deriv += (seq_t)(orig / temp);
    }
    return deriv;
}

/* Public sequence implementation. */
seq_t a(idx_t n)
{
    seq_t denom = primorial_exp(n);
    seq_t deriv = arithmetic_derivative(n);
    seq_t g = gcd(denom, deriv);
    return denom / g;
}
