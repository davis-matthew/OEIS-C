/*
 * A358683  a(n) is the sum of all divisors of all positive integers k where
 *          A182986(n) < k <= prime(n), n >= 1.
 * https://oeis.org/A358683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k = prime(n-1)+1}^{prime(n)} sigma(k) (prime(0)=0)
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // undefined for n=0, return 0 as safe default
    idx_t r = get_prime(n);        // prime(n)
    idx_t l;
    if (n == 1)                    // prime(0) is defined as 0
        l = 1;
    else
        l = get_prime(n - 1) + 1;  // lower limit

    seq_t sum = 0;
    for (idx_t k = l; k <= r; ++k)
        sum += sigma_of(k);
    return sum;
}
