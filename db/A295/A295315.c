/*
 * A295315  a(n) = phi(sigma(n)) / gcd(sigma(n), phi(sigma(n))).
 * https://oeis.org/A295315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(sigma(n)) / gcd(sigma(n), phi(sigma(n)))
 * Range:     u8 n <= 143, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 115 terms through n = 8752 (2026-11-01)
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
    /* sigma(n) */
    seq_t sigma = sigma_of(n);
    /* phi(sigma(n)) */
    seq_t phi_sigma = totient_seq(sigma);
    /* gcd(sigma, phi_sigma) */
    seq_t g = gcd(sigma, phi_sigma);
    /* a(n) = phi_sigma / g */
    return phi_sigma / g;
}
