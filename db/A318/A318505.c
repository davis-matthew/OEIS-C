/*
 * A318505  Sum of divisors of n, up to, but not including the second largest
 *          of them A032742(n); a(1) = 0 by convention.
 * https://oeis.org/A318505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(n) – n – A032742(n) = sum of all divisors of n minus n
 *            and minus its second largest divisor (which equals n /
 *            smallest_prime_factor(n)). a(1) = 0 by convention
 * Range:     u8 n <= 179, u16 n <= 9506, u32 n <= 9506, u64 n <= 9506,
 *            u128 n <= 9506, big any n
 * Helpers:   sigma_of, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* sum of all divisors of n */
    seq_t sigma = sigma_of(n);

    /* smallest prime factor of n (for n > 1) */
    uint64_t spf = smallest_prime_factor(n);

    /* second largest divisor = n / spf */
    uint64_t second_val = n / spf;
    seq_t second = (seq_t)second_val;

    /* subtract n and the second largest divisor */
    sigma -= (seq_t)n;
    sigma -= second;

    return sigma;
}
