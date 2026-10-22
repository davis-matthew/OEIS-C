/*
 * A395740  Number of iterations for the n-th prime to reach 2 under the
 *          function f(p) = smallest odd prime factor of 3*p+1 or 2 if 3*p+1
 *          is a power of 2.
 * https://oeis.org/A395740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto, prime_limit_for_idx, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/smallest_prime_factor.h"

/* Computed by simulating the iteration process for the n-th prime. */

static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;

/* Ensure prime_cache holds at least n primes. */
static void
ensure_primes(idx_t n)
{
    if (prime_cache_cnt >= (size_t)n)
        return;

    /* free previous cache */
    free(prime_cache);
    prime_cache = NULL;

    idx_t limit = prime_limit_for_idx(n);
    size_t cnt = 0;
    uint64_t *tmp = NULL;

    build_primes_upto(limit, &tmp, &cnt);

    prime_cache = tmp;
    prime_cache_cnt = cnt;
}

/* Wrapper for the provided smallest_prime_factor function. */
static uint64_t
smallest_odd_prime_factor(uint64_t n)
{
    return smallest_prime_factor(n);
}

seq_t
a(idx_t n)
{
    /* a(1) corresponds to the prime 2, which is already 2. */
    if (n == 1)
        return 0;

    ensure_primes(n);

    uint64_t p = prime_cache[n - 1];
    seq_t steps = 0;

    while (p != 2ULL) {
        uint64_t m = 3ULL * p + 1ULL;

        /* check whether m is a power of two */
        if ((m & (m - 1ULL)) == 0ULL) {
            p = 2ULL;
        } else {
            uint64_t odd = m;
            while ((odd & 1ULL) == 0ULL)
                odd >>= 1ULL;
            p = smallest_odd_prime_factor(odd);
        }
        steps++;
    }

    return steps;
}
