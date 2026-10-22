/*
 * A356472  Numerator of the average of gcd(i,n) for i = 1..n.
 * https://oeis.org/A356472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator( sum_{i=1}^n gcd(i,n) / n ) = sum_{i=1}^n
 *            gcd(i,n) / gcd( sum_{i=1}^n gcd(i,n), n )
 * Range:     u8 n <= 55, u16 n <= 6489, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute Euler's totient φ(m) for uint64_t m */
static seq_t phi_uint64(uint64_t m)
{
    seq_t res = (seq_t)m;
    uint64_t temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            res -= res / p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) res -= res / temp;
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but guard */
    uint64_t N = (uint64_t)n;           /* cast to native size for loops */
    seq_t sum = 0;

    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            uint64_t d2 = N / d;
            seq_t phi1 = phi_uint64(d2);
            sum += (seq_t)d * phi1;
            if (d2 != d) {
                seq_t phi2 = phi_uint64(d);
                sum += (seq_t)d2 * phi2;
            }
        }
    }

    seq_t g = gcd(sum, (seq_t)N);
    return sum / g;
}
