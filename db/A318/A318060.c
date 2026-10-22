/*
 * A318060  a(n) is the denominator of sigma(sigma(n))/n.
 * https://oeis.org/A318060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, σ(σ(n))) where σ is the sum‑of‑divisors
 *            function
 * Range:     u8 n <= 53, u16 n <= 7539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute σ(n) */
    seq_t sigma_n = sigma_of(n);

    /* Compute σ(σ(n)) – safe cast because n≤10 000 in tests and σ(n) fits in idx_t */
    seq_t sigma_sigma_n = sigma_of((idx_t)sigma_n);

    /* Compute denominator: n / gcd(n, σ(σ(n))) */
    seq_t g = gcd((seq_t)n, sigma_sigma_n);
    seq_t result = (seq_t)n / g;
    return result;
}
