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
 * Range:     u8 n <= 119, u16 n <= 9849, u32 n <= 9849, u64 n <= 9849,
 *            u128 n <= 9849, big any n
 * Helpers:   _smallest_prime_factor, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;                       /* sigma(1) = 1, A028234(1) = 1 → 0 */

    /* sum of divisors of n */
    seq_t sigma_n = sigma_of(n);

    /* smallest prime factor of n */
    uint64_t p = _smallest_prime_factor(n);

    /* remove all occurrences of p */
    uint64_t m = n;
    if (p > 1)
        while (m % p == 0)
            m /= p;

    seq_t sigma_m = sigma_of(m);

    return sigma_n - sigma_m;
}
