/*
 * A348969  a(n) = n / gcd(n, A099377(n)).
 * https://oeis.org/A348969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, A099377(n)), where A099377(n) =
 *            n*tau(n)/gcd(n*tau(n), sigma(n))
 * Range:     u8 n <= 35, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A099377_a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t tau = 1;
    seq_t sigma = 1;
    idx_t m = n;
    idx_t p = 2;

    while (p <= m / p) {
        if (m % p == 0) {
            idx_t e = 0;
            seq_t p_pow = 1;
            while (m % p == 0) {
                m /= p;
                ++e;
                p_pow *= (seq_t)p;
            }
            tau *= (seq_t)(e + 1);
            seq_t top = p_pow * (seq_t)p - (seq_t)1;
            seq_t denom = (seq_t)p - (seq_t)1;
            sigma *= top / denom;
        }
        ++p;
    }
    if (m > 1) {
        tau *= (seq_t)2;
        sigma *= ((seq_t)m + (seq_t)1);
    }
    seq_t num = nn * tau;
    seq_t g = gcd(num, sigma);
    return num / g;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t h = A099377_a(n);
    seq_t g = gcd(nn, h);
    return nn / g;
}
