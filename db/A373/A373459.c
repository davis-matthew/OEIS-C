/*
 * A373459  Expansion of Sum_{p prime} x^p/(1 - (p*x)^p).
 * https://oeis.org/A373459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, prime} p^(n-p)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 19, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   build_primes_upto, floor_isqrt_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* Cache the primes that are needed for all calls */
    static uint64_t *prime_cache = NULL;
    static size_t      prime_cnt  = 0;
    static idx_t       prime_limit = 0;

    idx_t sqrt_n = floor_isqrt_u64(n);
    if (sqrt_n > prime_limit) {
        /* Rebuild prime table for the larger limit */
        free(prime_cache);
        build_primes_upto(sqrt_n, &prime_cache, &prime_cnt);
        prime_limit = sqrt_n;
    }

    seq_t sum = (seq_t)0;
    uint64_t temp = n;

    /* Try all primes up to sqrt(n) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > sqrt_n)
            break;
        if (temp % p == 0) {
            sum += pow_uint(p, n - p);
            /* Remove all occurrences of this prime factor to avoid double‑counting */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* Remaining factor > sqrt(n) is either 1 or a single prime > sqrt(n) */
    if (temp > 1) {
        /* If n is prime, temp == n and exponent is n-n = 0 -> 1 */
        sum += pow_uint(temp, n - temp);
    }

    return sum;
}
