/*
 * A306530  a(n) is the smallest prime q such that Kronecker(q, prime(n)) = 1.
 * https://oeis.org/A306530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime q such that Kronecker(q, prime(n)) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Utility headers */
#include "../utils/get_prime.h"         /* uint32_t get_prime(idx_t k); */
#include "../utils/jacobi_symbol.h"     /* int jacobi_symbol(uint64_t a, uint64_t n); */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n); */

/* --------------------------------------------------------------------------
 * Prime cache – stores small primes (uint64_t).  The cache is expanded
 * automatically when larger primes are needed during the search.
 * -------------------------------------------------------------------------- */
static uint64_t *prime_cache = NULL;   /* array of primes, ascending order   */
static size_t   prime_cnt   = 0;       /* number of primes stored            */
static size_t   prime_cap   = 0;       /* allocated capacity                 */

/* Ensure capacity for at least 'new_cap' primes */
static void prime_ensure_capacity(size_t new_cap)
{
    if (prime_cap == 0) {
        prime_cap = (new_cap > 8) ? new_cap : 8;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache) abort();
    } else if (prime_cap < new_cap) {
        size_t new_cap2 = prime_cap * 2;
        if (new_cap2 < new_cap) new_cap2 = new_cap;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap2 * sizeof(uint64_t));
        if (!tmp) abort();
        prime_cache = tmp;
        prime_cap   = new_cap2;
    }
}

/* Append a new prime to the cache (assumed to be > last stored) */
static void prime_add(uint64_t p)
{
    prime_ensure_capacity(prime_cnt + 1);
    prime_cache[prime_cnt++] = p;
}

/* Generate the next prime after 'last'; skips even numbers */
static uint64_t prime_next(uint64_t last)
{
    uint64_t cand = last + 1;
    if (cand % 2ULL == 0ULL && cand != 2ULL) cand++; /* skip even */
    for (;;) {
        if (cand == 2ULL) return 2ULL;
        if (is_prime_big((seq_t)cand)) return cand;
        cand += 2ULL; /* only odd numbers after 2 */
    }
}

/* --------------------------------------------------------------------------
 * Main sequence function
 * -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialize prime cache with the first prime 2 */
    if (prime_cnt == 0) prime_add(2);

    /* Fetch the n-th prime (1‑based).  p fits in uint32_t for n ≤ 10000. */
    uint32_t p32 = get_prime(n);
    uint64_t  p   = (uint64_t)p32;   /* promote to 64‑bit for uniformity */

    /* Special handling for p = 2: Kronecker(q,2)=1 ⇔ q ≡ ±1 (mod 8). */
    if (p == 2ULL) {
        for (size_t idx = 0;; ++idx) {
            if (idx >= prime_cnt) {
                uint64_t last = prime_cache[prime_cnt - 1];
                prime_add(prime_next(last));
            }
            uint64_t q = prime_cache[idx];
            if (q == 2ULL) continue; /* skip q = 2, which is even */
            if ((q & 7ULL) == 1ULL || (q & 7ULL) == 7ULL) {
                return (seq_t)q;
            }
        }
    }

    /* For odd prime p, we look for the smallest prime q with Legendre symbol (q|p)=1. */
    for (size_t idx = 0;; ++idx) {
        if (idx >= prime_cnt) {
            uint64_t last = prime_cache[prime_cnt - 1];
            prime_add(prime_next(last));
        }
        uint64_t q = prime_cache[idx];
        /* (q|p) = 0 when q = p; skip those. */
        if (q == p) continue;
        uint64_t rem = q % p;              /* reduce modulo p – fits in 64‑bit */
        if (jacobi_symbol(rem, p) == 1) {
            return (seq_t)q;
        }
    }
}
