/*
 * A289497  Number of primes in the interval [6n, 7n].
 * https://oeis.org/A289497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes in the interval [6n, 7n]
 * Range:     u8 n <= 2434, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Auxiliary: integer square root */
static idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) {
        ++r;
    }
    return r;
}

/* Global prime cache */
static idx_t *prime_list = NULL;   /* array of primes */
static size_t prime_count = 0;     /* number of primes stored */
static idx_t prime_limit = 0;      /* highest number sieved */

/* Ensure that all primes up to limit are sieved and stored */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* Allocate sieve array */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_prime) return;               /* allocation failure: leave primes unchanged */

    memset(is_prime, 1, (size_t)(limit + 1));
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    idx_t sqrt_limit = int_sqrt(limit);
    for (idx_t p = 2; p <= sqrt_limit; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t multiple = p * p; multiple <= limit; multiple += p) {
            is_prime[multiple] = 0;
        }
    }

    /* Count primes */
    size_t new_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) ++new_count;
    }

    /* Reallocate prime list */
    idx_t *new_list = (idx_t *)realloc(prime_list, new_count * sizeof(idx_t));
    if (!new_list) {  /* allocation failure: revert */
        free(is_prime);
        return;
    }
    prime_list = new_list;

    /* Fill prime list */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) prime_list[idx++] = i;
    }

    prime_count = new_count;
    prime_limit = limit;
    free(is_prime);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t lower = 6 * n;
    idx_t upper = 7 * n;
    ensure_primes(upper);

    /* Find first prime >= lower */
    size_t lo = 0;
    size_t hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (prime_list[mid] < lower) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    size_t start_idx = lo;

    /* Find first prime > upper */
    lo = 0;
    hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (prime_list[mid] <= upper) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    size_t end_idx = lo;

    /* Count of primes in [lower, upper] */
    seq_t count = (seq_t)(end_idx - start_idx);
    return count;
}
