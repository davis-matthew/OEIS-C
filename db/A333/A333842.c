/*
 * A333842  G.f.: Sum_{k>=1} k * x^(prime(k)^2) / (1 - x^(prime(k)^2)).
 * https://oeis.org/A333842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p^e||n, e>=2} primepi(p), where primepi(p) = number
 *            of primes <= p. For n=1,2,3 the value is 0
 * Range:     u8 n <= 9335, u16 n <= 9335, u32 n <= 9335, u64 n <= 9335,
 *            u128 n <= 9335, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"

/* integer square root for uint64_t:  floor(sqrt(n)). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n <= 3) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    seq_t sum = 0;
    idx_t limit = isqrt_u64(m);
    for (uint64_t p = 2; p <= limit; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp >= 2) {
                sum += compute_pi(p);
            }
            limit = isqrt_u64(m);  /* adjust limit for remaining factor */
        }
    }
    /* Remaining m > 1 is a prime with exponent 1: no contribution. */
    return sum;
}
