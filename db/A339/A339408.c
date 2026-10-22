/*
 * A339408  Number of compositions (ordered partitions) of n into an even
 *          number of primes.
 * https://oeis.org/A339408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of compositions of n into an even number of primes. */
/* Computes a(n) as defined by OEIS A339408. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Generate all primes up to "limit" (inclusive) and return them in a
   dynamically allocated array. The caller receives the array pointer
   and its length. */
static void generate_primes_upto(idx_t limit, uint64_t **primes_out,
                                 size_t *cnt_out)
{
    size_t sz = limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        return;                       /* Out of memory: return empty list */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++cnt;

    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            pr[k++] = (uint64_t)i;

    free(is_prime);
    *primes_out = pr;
    *cnt_out = cnt;
}

/* Global static state for the DP and primes cache */
static seq_t *even_cache = NULL;
static seq_t *odd_cache  = NULL;
static uint64_t *primes  = NULL;
static size_t   prime_cnt = 0;
static idx_t   prime_limit = 0;
static idx_t   dp_next = 0;   /* next index we need to compute (up to dp_next-1 already computed) */
static idx_t   dp_size = 0;   /* allocated size of the caches */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure cache large enough for n */
    if (dp_size <= n) {
        size_t new_size = n + 1;
        even_cache = (seq_t *)realloc(even_cache, new_size * sizeof(seq_t));
        odd_cache  = (seq_t *)realloc(odd_cache,  new_size * sizeof(seq_t));
        for (idx_t i = dp_size; i <= n; ++i) {
            even_cache[i] = 0;
            odd_cache[i]  = 0;
        }
        dp_size = new_size;
    }

    /* Update primes if needed */
    if (prime_limit < n) {
        free(primes);
        primes = NULL;
        prime_cnt = 0;
        generate_primes_upto(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    /* Base case: even[0] = 1, odd[0] = 0 */
    if (dp_next == 0) {
        even_cache[0] = 1;
        odd_cache[0]  = 0;
        dp_next = 1;
    }

    /* Compute DP entries from dp_next up to n */
    for (idx_t i = dp_next; i <= n; ++i) {
        seq_t sum_even = 0;
        seq_t sum_odd  = 0;
        for (size_t pi = 0; pi < prime_cnt && primes[pi] <= i; ++pi) {
            idx_t rem = i - (idx_t)primes[pi];
            sum_even += odd_cache[rem];
            sum_odd  += even_cache[rem];
        }
        even_cache[i] = sum_even;
        odd_cache[i]  = sum_odd;
    }
    dp_next = n + 1;

    return even_cache[n];
}
