/*
 * A341637  a(n) = Sum_{d|n} phi(d) * sigma(d) * sigma(n/d).
 * https://oeis.org/A341637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} phi(d) * sigma(d) * sigma(n/d)
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of_value.h"
static inline seq_t totient(idx_t m)
{
    seq_t phi = m;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            phi -= phi / (seq_t)p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) phi -= phi / (seq_t)temp;
    return phi;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            seq_t phi1 = totient(d1);
            seq_t sigma1 = sigma_of_value(d1);
            seq_t sigma2 = sigma_of_value(d2);

            result += phi1 * sigma1 * sigma2;
            if (d1 != d2) {
                seq_t phi2 = totient(d2);
                result += phi2 * sigma2 * sigma1;
            }
        }
    }
    return result;
}
