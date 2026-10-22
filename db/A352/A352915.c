/*
 * A352915  Moebius transform of odd primes.
 * https://oeis.org/A352915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * prime(d+1)
 * Range:     u8 n <= 58, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    long long sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d1;
            int mu1 = mu_of(n / d1);
            if (mu1 != 0) {
                sum += (long long)mu1 * (long long)get_prime(d1 + 1);
            }
            if (d1 != d2) {
                int mu2 = mu_of(n / d2);
                if (mu2 != 0) {
                    sum += (long long)mu2 * (long long)get_prime(d2 + 1);
                }
            }
        }
    }
    return (seq_t)sum;
}
