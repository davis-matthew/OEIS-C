/*
 * A272173  Product of the sum of the divisors of n and the sum of the
 *          divisors of n-th prime.
 * https://oeis.org/A272173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) * sigma(prime(n))
 * Range:     u8 n <= 7, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sigma_n = sigma_of(n);
    uint32_t p = get_prime(n);
    seq_t sigma_p = sigma_of((idx_t)p);
    return sigma_n * sigma_p;
}
