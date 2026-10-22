/*
 * A361761  a(n) = Product_{i=prime(n)..prime(n+1)} i.
 * https://oeis.org/A361761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i = prime(n)}^{prime(n+1)} i (product of all integers
 *            from the nth prime to the next prime)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 29, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache of primes (0‑based: primes[0] = 2). */
static uint64_t *prime_cache   = NULL;   /* array of primes */
static size_t   prime_cnt      = 0;       /* number of primes stored */
static size_t   prime_cap      = 0;       /* allocated capacity */

/* Ensure that prime_cache contains primes up to index 'upto_idx' (inclusive). */
static void ensure_primes_upto(idx_t upto_idx)
{
    /* We need up to index upto_idx, so prime_cnt must be > upto_idx. */
    if (prime_cnt > upto_idx)
        return;                 /* already have enough */

    /* Grow the buffer if necessary. */
    size_t needed = upto_idx + 1;           /* number of primes needed */
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp)
            exit(EXIT_FAILURE);  /* insufficient memory */
        prime_cache = tmp;
        prime_cap   = new_cap;
    }

    /* Start searching for primes from the next candidate. */
    uint64_t candidate = prime_cnt ? prime_cache[prime_cnt - 1] + 1 : 2;
    for (; prime_cnt <= upto_idx; ++candidate) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > candidate)       /* p > sqrt(candidate) */
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            prime_cache[prime_cnt++] = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                      /* sequence is defined for n ≥ 1; safety return */
        return (seq_t)0;

    /* Ensure prime(n) and prime(n+1) are available. */
    ensure_primes_upto(n);           /* need primes up to index n (zero‑based). */

    uint64_t p_start = prime_cache[(size_t)n - 1]; /* prime(n)      */
    uint64_t p_end   = prime_cache[(size_t)n];     /* prime(n+1)    */

    seq_t prod = (seq_t)1;
    for (uint64_t x = p_start; x <= p_end; ++x)
        prod *= (seq_t)x;

    return prod;
}
