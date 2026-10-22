/*
 * A305054  If n = Product_i prime(x_i)^k_i, then a(n) = Sum_i k_i *
 *          omega(x_i), where omega = A001221 is number of distinct prime
 *          factors.
 * https://oeis.org/A305054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ k_i * ω(i), where n = Π prime(i)^k_i and ω(i) is the
 *            number of distinct prime factors of the integer i (the index of
 *            the prime)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, count_distinct_primes,
 *            prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/prime_pi.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* a(1) = 0, safe for other values */

    /* Static cache of primes up to the largest √n seen so far. */
    static uint64_t *primes = NULL;
    static size_t   prime_cnt = 0;
    static idx_t    current_limit = 0;

    idx_t sqrt_n = _floor_isqrt_u64(n);
    if (sqrt_n > current_limit)
    {
        free(primes);                     /* release old list if any */
        build_primes_upto(sqrt_n, &primes, &prime_cnt);
        current_limit = sqrt_n;
        if (!primes) return 0;            /* allocation failure fallback   */
    }

    idx_t temp = n;
    seq_t result = 0;

    /* Factorization via trial division using the cached primes. */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp) break;

        if (temp % p == 0)
        {
            seq_t exp = 0;
            do
            {
                temp /= p;
                ++exp;
            } while (temp % p == 0);

            idx_t idx = (idx_t)prime_pi(p);                     /* index of prime p */
            seq_t omega = (seq_t)count_distinct_primes((uint64_t)idx); /* ω(idx)  */
            result += exp * omega;
        }
    }

    /* Remaining prime factor > √n, if any. */
    if (temp > 1)
    {
        uint64_t p = temp;
        seq_t exp = 1;
        idx_t idx = (idx_t)prime_pi(p);
        seq_t omega = (seq_t)count_distinct_primes((uint64_t)idx);
        result += exp * omega;
    }

    return result;
}
