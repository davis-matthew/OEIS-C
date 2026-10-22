/*
 * A344987  Number of Goldbach partitions of 2n into 2 primes where the
 *          smaller prime has an odd index and the larger prime has an even
 *          index.
 * https://oeis.org/A344987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of Goldbach partitions of 2*n into two primes
 * where the smaller prime has an odd index (π(p) odd) and the larger prime
 * has an even index (π(q) even).  The function directly implements that
 * definition via a sieve of Eratosthenes, prime counting array and a scan
 * over all prime candidates.  It caches the sieve so each n need only
 * rebuild the array when a larger limit is requested.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache data */
static uint8_t  *is_prime   = NULL;      /* 0/1 flag array */
static uint64_t *pi_arr     = NULL;      /* π(k) values */
static uint32_t *prime_list = NULL;      /* list of primes (uint32_t) */
static size_t    prime_cnt   = 0;         /* number of primes */
static size_t    max_limit   = 0;         /* current size of cache */

/* Build or extend the sieve up to new_limit (inclusive) */
static void rebuild_sieve(size_t new_limit)
{
    /* Free previous cache */
    free(is_prime);
    free(pi_arr);
    free(prime_list);

    /* Allocate new arrays */
    is_prime   = (uint8_t *)calloc(new_limit + 1, sizeof(uint8_t));
    pi_arr     = (uint64_t *)malloc((new_limit + 1) * sizeof(uint64_t));
    /* prime_list will be allocated after we know how many primes */

    if (!is_prime || !pi_arr) {
        /* Allocation failed – abort */
        exit(EXIT_FAILURE);
    }

    /* Initialise all to prime (1), then clear 0 and 1 */
    for (size_t i = 2; i <= new_limit; ++i)
        is_prime[i] = 1;
    if (new_limit >= 0) is_prime[0] = 0;
    if (new_limit >= 1) is_prime[1] = 0;

    /* Eratosthenes sieve */
    for (size_t p = 2; p * p <= new_limit; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= new_limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Build π(k) array and count primes */
    uint64_t cnt = 0;
    for (size_t i = 0; i <= new_limit; ++i) {
        if (is_prime[i]) ++cnt;
        pi_arr[i] = cnt;
    }
    prime_cnt = (size_t)cnt;

    /* Allocate and fill prime list */
    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint32_t)i;

    max_limit = new_limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Target Goldbach number is 2*n */
    size_t limit = (size_t)(n * 2ull);

    /* Rebuild cache if needed */
    if (limit > max_limit)
        rebuild_sieve(limit);

    seq_t result = 0;

    /* Scan all primes p <= n */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((seq_t)p > n) break;

        uint32_t q = (uint32_t)(limit - p);  /* larger prime candidate */

        /* Check that q is prime */
        if (q > limit || !is_prime[q]) continue;

        /* π(p) odd, π(q) even */
        if ((pi_arr[p] & 1u) && !(pi_arr[q] & 1u))
            ++result;
    }

    return result;
}
