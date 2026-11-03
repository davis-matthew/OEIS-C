/*
 * A295311  a(n) = n / A295310(n) = n / gcd(n, phi(sigma(n))).
 * https://oeis.org/A295311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, phi(sigma(n)))
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 9615 (2026-11-01)
 * Helpers:   gcd, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient_seq.h"

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
    /* sum of divisors of n */
    seq_t sigma = sigma_of(n);
    /* Euler's totient of sigma */
    seq_t phi_val = totient_seq(sigma);
    /* gcd(n, phi(sigma)) */
    seq_t g = gcd((seq_t)n, phi_val);
    /* return n / g */
    return (seq_t)n / g;
}
