/*
 * A279287  a(n) = numerator of (phi(n)/tau(n)).
 * https://oeis.org/A279287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of phi(n)/tau(n)
 * Range:     u8 n <= 282, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Euler's totient function, computed by factorisation. */
static seq_t totient(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = (seq_t)n;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            result -= result / p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

/* Divisor count τ(n), computed by factorisation. */
static seq_t divisor_count(idx_t n)
{
    if (n <= 1) return 1;
    seq_t tau = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            tau *= (seq_t)(exp + 1);
        }
    }
    if (m > 1) tau *= 2;
    return tau;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* behaviour below offset irrelevant */
    seq_t phi   = totient(n);
    seq_t tau   = divisor_count(n);
    seq_t g     = gcd(phi, tau);
    return phi / g;                      /* numerator of the reduced fraction */
}
