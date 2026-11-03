/*
 * A386402  a(n) = denominator(sigma(n)*phi(n)/n^2).
 * https://oeis.org/A386402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator(sigma(n)*phi(n)/n^2)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9848 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

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

seq_t a(idx_t n) {
    seq_t sigma_n = sigma_of(n);
    seq_t phi_n   = totient(n);
    seq_t denom   = (seq_t)n * (seq_t)n;
    seq_t numer   = sigma_n * phi_n;
    seq_t g       = gcd(numer, denom);
    return denom / g;
}
