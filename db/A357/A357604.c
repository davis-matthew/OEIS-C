/*
 * A357604  Number of prime powers in the sequence of the floor of n/k for k
 *          <= n, A010766.
 * https://oeis.org/A357604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k (1 ≤ k ≤ n) such that floor(n/k) is a prime
 *            power
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Cache of whether a number is a prime power: 1 if true, 0 otherwise. */
static unsigned char *prime_power_cache = NULL;
/* Highest value currently covered by the cache. */
static idx_t cache_limit = 0;

/* Extend the cache so that it contains data for all values up to n. */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n <= cache_limit)
        return;

    /* Allocate a fresh cache large enough for all values ≤ n. */
    size_t new_size = (size_t)(n + 1);
    unsigned char *new_cache = (unsigned char *)calloc(new_size, sizeof(unsigned char));
    if (!new_cache) {
        /* Allocation failure: abort the program (the OEIS harness expects a crash). */
        abort();
    }

    /* If we already had a cache, copy its contents and free the old array. */
    if (cache_limit > 0) {
        memcpy(new_cache, prime_power_cache, (size_t)(cache_limit + 1));
        free(prime_power_cache);
    }

    /* Sieve of Eratosthenes to find primes up to n. */
    unsigned char *is_prime = (unsigned char *)calloc(new_size, sizeof(unsigned char));
    if (!is_prime) {
        free(new_cache);
        abort();
    }
    for (idx_t i = 2; i <= n; ++i)
        is_prime[i] = 1;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = 0;
        }
    }

    /* Mark all prime powers ≤ n. */
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime[p])
            continue;
        idx_t power = (idx_t)p;
        while (1) {
            new_cache[power] = 1;
            if (power > n / p)   /* the next multiplication would overflow or exceed n */
                break;
            power *= p;
        }
    }

    free(is_prime);
    prime_power_cache = new_cache;
    cache_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_cache(n);

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n / k;                     /* floor(n/k) */
        if (prime_power_cache[m])
            ++count;
    }
    return count;
}
