/*
 * A335285  a(n) is the greatest possible greatest part of any partition of n
 *          into prime parts.
 * https://oeis.org/A335285
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the greatest prime ≤ n for odd n, and the greatest prime
 *            ≤ n–2 for even n>2. For n = 2 the value is 2. The implementation
 *            builds a prime sieve up to the largest argument seen and
 *            performs binary searches on the stored prime list
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static state for the prime table */
static uint64_t *primes = NULL;
static size_t   prime_cnt = 0;
static uint64_t prime_max  = 0;

/* Build a sieve of Eratosthenes up to `limit` and store all primes in `primes`. */
static void build_primes_upto(uint64_t limit)
{
    if (limit <= prime_max) return;          /* already has all needed primes */

    /* Simple sieve; size is small for typical n (≤ 10000) */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) abort();

    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    /* Count primes to allocate array */
    size_t tmp_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++tmp_cnt;

    if (primes == NULL) {
        primes = (uint64_t *)malloc(tmp_cnt * sizeof(uint64_t));
    } else {
        primes = (uint64_t *)realloc(primes, tmp_cnt * sizeof(uint64_t));
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = i;

    prime_cnt  = tmp_cnt;
    prime_max  = limit;
    free(is_prime);
}

/* Return the largest prime ≤ x (or 0 if none).  `x` fits in uint64_t. */
static seq_t largest_prime_le(seq_t x)
{
    if (x < 2) return 0;

    /* Ensure primes up to x */
    build_primes_upto((uint64_t)x);

    /* Binary search for upper bound */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= (uint64_t)x)
            lo = mid + 1;
        else
            hi = mid;
    }

    if (lo == 0) return 0;
    return (seq_t)primes[lo - 1];
}

/* Public function for OEIS A335285 */
seq_t a(idx_t n)
{
    /* Only defined for n ≥ 2; behaviour below offset is irrelevant */
    if (n == 2) return (seq_t)2;

    if (n & 1) {                /* n is odd */
        return largest_prime_le((seq_t)n);
    } else {                    /* n is even > 2 */
        uint64_t limit = (uint64_t)(n - 2);
        return largest_prime_le((seq_t)limit);
    }
}
