/*
 * A333700  a(n) = Sum_{k=1..n} pi(k) * pi(n-k).
 * https://oeis.org/A333700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} pi(k) * pi(n-k)
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence for all n >= 1. Uses an integer cache for pi(k). */

#include <stdlib.h>
#include <stdbool.h>

/* Integer cache for prime-counting values */
static uint64_t *pi_cache = NULL;      /* pi_cache[i] = pi(i) for i up to pi_cache_size-1 */
static idx_t  pi_cache_size = 0;       /* current size of pi_cache */

/* Build or extend the pi cache up to index n (inclusive). */
static void ensure_pi_cache(idx_t n)
{
    /* Nothing to do if the cache already covers n */
    if (pi_cache_size > n) return;

    /* Allocate new cache of size n+1 */
    if (pi_cache) free(pi_cache);
    pi_cache = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!pi_cache) exit(1);                 /* Out of memory: abort */
    pi_cache_size = n + 1;

    /* Simple sieve of Eratosthenes to find primes up to n */
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!is_prime) { free(pi_cache); exit(1); }

    for (idx_t i = 0; i <= n; ++i) is_prime[i] = (i >= 2);

    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t mult = p * p; mult <= n; mult += p)
                is_prime[mult] = false;
        }
    }

    /* Build cumulative prime counts */
    pi_cache[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        pi_cache[i] = pi_cache[i - 1] + (is_prime[i] ? 1 : 0);
    }

    free(is_prime);
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    ensure_pi_cache(n);

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pk  = (seq_t)pi_cache[k];
        seq_t pnk = (seq_t)pi_cache[n - k];
        sum += pk * pnk;
    }
    return sum;
}
