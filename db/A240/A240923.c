/*
 * A240923  a(n) = numerator(sigma(n)/n) - sigma(denominator(sigma(n)/n)).
 * https://oeis.org/A240923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n)/gcd(n,sigma(n)) - sigma(n/gcd(n,sigma(n)))
 * Range:     u8 n <= 364, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9656 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

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
    /* sigma_of returns σ(n) as a seq_t */
    seq_t sigma_n = sigma_of(n);
    /* g = gcd(n, σ(n)) */
    seq_t g = gcd((seq_t)n, sigma_n);

    /* n_over_g = n / g, σ_n_over_g = σ(n) / g */
    seq_t n_over_g = (seq_t)n / g;
    seq_t sigma_n_over_g = sigma_n / g;

    /* σ( n/g ) */
    seq_t sigma_d = sigma_of((idx_t)n_over_g);

    /* a(n) = σ(n)/g - σ(n/g) */
    seq_t result = sigma_n_over_g;
    result -= sigma_d;
    return result;
}
