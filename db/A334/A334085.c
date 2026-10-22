/*
 * A334085  GCD of n and the product of all primes < n.
 * https://oeis.org/A334085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n, product of all primes < n); Equivalent to a(1)=1,
 *            a(n)=1 if n is prime, else a(n)=product of distinct prime
 *            factors of n
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd, int_sqrt, is_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: all helper functions used below are provided in "../utils/*.h". */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Cache array: 0 means not yet computed (valid because sequence values are never 0). */
    if (n >= cache_size) {
        size_t new_size = (cache_size == 0) ? 16 : cache_size;
        while (n >= new_size) new_size <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;  /* Out of memory – the caller should handle this. */
        /* Zero-fill the newly allocated portion */
        for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
    }
    if (cache[n]) return cache[n];   /* value already computed */

    /* Compute a(n) */
    seq_t result = (seq_t)1;          /* start with 1 (neutral for multiplication) */

    if (n <= 1) {
        result = (seq_t)1;            /* a(1) = 1 */
    } else if (is_prime(n)) {
        result = (seq_t)1;            /* primes give value 1 */
    } else {
        /* Factor n, multiply distinct prime factors */
        static uint64_t *prime_list = NULL;
        static size_t prime_count = 0;
        static idx_t prime_limit = 0;

        idx_t limit = int_sqrt(n);    /* sqrt(n) is enough to test all prime divisors */
        if (limit > prime_limit) {
            /* Build primes up to the new limit; free previous list if any */
            if (prime_list) free(prime_list);
            build_primes_upto(limit, &prime_list, &prime_count);
            prime_limit = limit;
        }

        uint64_t temp = n;
        for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] <= limit; ++i) {
            uint64_t p = prime_list[i];
            if (temp % p == 0) {
                result = seq_mul(result, (seq_t)p);  /* multiply distinct prime factor */
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) {                 /* remaining factor is a prime > sqrt(n) */
            result = seq_mul(result, (seq_t)temp);
        }
    }

    cache[n] = result;
    return result;
}
