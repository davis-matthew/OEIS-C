/*
 * A258168  Number of ways to write n as floor((p^2+q)/5) with p and q both
 *          prime.
 * https://oeis.org/A258168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A258168: number of ordered prime pairs (p,q) such that
   floor((p^2+q)/5) == n.  This is done by enumerating, for each
   k = 5*n,5*n+1,…,5*n+4, all primes p with p^2 <= k and testing
   whether q = k - p^2 is prime.  The result is returned as a seq_t.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static prime cache */
static idx_t sieve_max      = 0;
static uint8_t *is_prime_arr = NULL;
static uint32_t *primes_arr  = NULL;
static size_t   primes_count = 0;
static size_t   primes_cap   = 0;

/* Ensure prime table up to 'limit' is available.  The table is
   recomputed only when a larger limit is required. */
static void ensure_prime_cache(idx_t n)
{
    idx_t limit = 5 * n + 4;
    if (limit <= sieve_max)
        return;

    /* free old tables */
    free(is_prime_arr);
    free(primes_arr);

    sieve_max = limit;
    size_t sz = (size_t)(limit + 1);
    is_prime_arr = (uint8_t *)malloc(sz);
    if (!is_prime_arr)
        abort();

    for (size_t i = 0; i < sz; ++i)
        is_prime_arr[i] = 1;
    is_prime_arr[0] = is_prime_arr[1] = 0;

    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (is_prime_arr[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime_arr[j] = 0;
        }
    }

    primes_cap   = (size_t)(sz / 10 + 10);
    primes_arr   = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes_arr)
        abort();
    primes_count = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime_arr[i]) {
            if (primes_count == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes_arr,
                                                    primes_cap * sizeof(uint32_t));
                if (!tmp)
                    abort();
                primes_arr = tmp;
            }
            primes_arr[primes_count++] = (uint32_t)i;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;         /* below offset, value irrelevant */

    ensure_prime_cache(n);

    idx_t k_start = 5 * n;
    seq_t count = 0;

    for (idx_t k = k_start; k <= k_start + 4; ++k) {
        for (size_t pi = 0; pi < primes_count; ++pi) {
            uint32_t p = primes_arr[pi];
            uint64_t p2 = (uint64_t)p * (uint64_t)p;
            if (p2 > k)
                break;
            uint64_t q = (uint64_t)k - p2;
            if (q < 2)
                continue;
            if (q <= sieve_max && is_prime_arr[q])
                ++count;
        }
    }
    return count;
}
