/*
 * A378201  a(n) = 2*n mod q, where q is the greatest prime such that 2*n - q
 *          is also prime.
 * https://oeis.org/A378201
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n mod q where q is the greatest prime such that 2*n - q
 *            is also prime
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/sieve_primes.h"

static idx_t *prime_cache = NULL;   /* array of primes up to current limit */
static size_t prime_cnt = 0;        /* number of primes in cache */
static idx_t prime_limit = 0;       /* maximum value for which primes are cached */

seq_t a(idx_t n)
{
    /* Ensure there is at least a minimal limit for n>=2 */
    if (n < 2) {
        return 0;
    }

    idx_t limit = n * 2;

    /* Resize prime cache if needed */
    if (limit > prime_limit) {
        if (prime_cache) {
            free(prime_cache);
        }
        sieve_primes(limit, &prime_cache, &prime_cnt);
        prime_limit = limit;
    }

    /* Search for the greatest prime q so that 2*n - q is prime */
    int64_t i = (int64_t)prime_cnt - 1;
    for (; i >= 0; --i) {
        idx_t p = prime_cache[i];
        if (p > limit) {
            continue;
        }
        idx_t r = limit - p;
        if (is_prime(r)) {
            seq_t two_n = (seq_t)n * 2;
            return two_n % (seq_t)p;
        }
    }

    /* Fallback: should not happen for n >= 2 */
    return 0;
}
