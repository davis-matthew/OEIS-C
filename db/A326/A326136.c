/*
 * A326136  a(n) = sigma(n) - sigma(A028234(n)), where sigma is the sum of
 *          divisors of n, and A028234 gives n without any occurrence of its
 *          smallest prime factor.
 * https://oeis.org/A326136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - sigma(A028234(n)) where A028234(n) removes all
 *            occurrences of the smallest prime factor of n
 * Range:     u8 n <= 119, u16 n <= 18479, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 9849 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/smallest_prime_factor.h"

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
    if (n == 1)
        return 0;                       /* sigma(1) = 1, A028234(1) = 1 → 0 */

    /* sum of divisors of n */
    seq_t sigma_n = sigma_of(n);

    /* smallest prime factor of n */
    uint64_t p = smallest_prime_factor(n);

    /* remove all occurrences of p */
    uint64_t m = n;
    if (p > 1)
        while (m % p == 0)
            m /= p;

    seq_t sigma_m = sigma_of(m);

    return sigma_n - sigma_m;
}
