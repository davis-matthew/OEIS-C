/*
 * A385229  Odd multiplicative orders of -3 modulo primes.
 * https://oeis.org/A385229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2537, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = odd multiplicative order of -3 modulo the nth prime
 * from OEIS A385223.  The sequence is obtained by enumerating primes,
 * computing the multiplicative order of (-3) mod p, and keeping only
 * the orders that are odd.  The result is cached for repeated calls. */

/* ------------------------------------------------------------------ */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t); (for idx_t=uint64_t) */
#include "../utils/mod_pow.h"       /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/seq_alloc.h"     /* seq_t *seq_alloc(size_t cap); */
#include "../utils/seq_realloc.h"   /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */
/* ------------------------------------------------------------------ */

#define PRIME_SIEVE_LIMIT 2000000     /* ensures enough primes for n≈10000 */

/* ------------------------------------------------------------------ */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* Smallest prime sieve, built once on first use. */
static void init_prime_list(void)
{
    if (prime_list != NULL)   /* already initialized */
        return;

    /* Allocate and initialize the sieve. */
    uint8_t *sieve = (uint8_t*)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    if (!sieve)            /* allocation failure – fall back to trial division */
        return;

    /* Mark primes. */
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        sieve[i] = 1;
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes to allocate list. */
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i])
            ++prime_count;

    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) {          /* allocation failure – free and abort init */
        free(sieve);
        prime_count = 0;
        return;
    }

    /* Fill the list. */
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i])
            prime_list[idx++] = (uint32_t)i;

    free(sieve);
}

/* ------------------------------------------------------------------ */
/* Simple primality test using the precomputed prime list. It is
   sufficient for the range of primes needed (n ≤ 10000). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if (n % 2 == 0)
        return false;
    init_prime_list();
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n)
            break;
        if (n % p == 0)
            return n == p;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Compute the multiplicative order of -3 modulo a prime p.  The order
   divides p-1, so we factor (p-1) using the prime list and reduce the
   order by each prime factor. */
static seq_t order_minus3(uint64_t p)
{
    if (p == 2)          /* -3 ≡ 1 (mod 2), order = 1 */
        return (seq_t)1;

    seq_t base = (seq_t)(p > 3 ? p - 3 : (p == 3 ? 0 : p - 3));
    seq_t order = (seq_t)(p - 1);   /* p is prime, so order | p-1 */

    init_prime_list();

    uint64_t m = p - 1;   /* factorization workspace */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t q = prime_list[i];
        if (q * q > m)
            break;
        if (m % q == 0) {
            while (m % q == 0)
                m /= q;          /* remove all copies of q */

            /* Reduce the order by q as long as possible. */
            while ((order % q == 0) &&
                   (mod_pow(base, (uint64_t)(order / q), (seq_t)p) == (seq_t)1))
                order /= q;
        }
    }

    /* If m > 1, it is a remaining prime factor > sqrt(p-1). */
    if (m > 1) {
        uint64_t q = m;
        while ((order % q == 0) &&
               (mod_pow(base, (uint64_t)(order / q), (seq_t)p) == (seq_t)1))
            order /= q;
    }

    return order;
}

/* ------------------------------------------------------------------ */
/* Cache for computed terms: array of seq_t.  The cache is enlarged
   on demand when more terms are requested. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored in cache */
static size_t cache_cap = 0;   /* capacity of the cache array */
static uint64_t last_checked_prime = 1;  /* prime up to which we have scanned */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined; return 0 to keep compiler happy */
        return (seq_t)0;

    /* Ensure cache holds at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next prime after last_checked_prime. */
        do {
            ++last_checked_prime;
        } while (!is_prime_uint64(last_checked_prime));

        /* Compute the odd multiplicative order of -3 modulo this prime. */
        seq_t ord = order_minus3(last_checked_prime);
        if ((ord & (seq_t)1) != 0) {  /* keep only odd orders */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (cache == NULL) {
                    cache = seq_alloc(new_cap);
                } else {
                    cache = seq_realloc(cache, cache_cap, new_cap);
                }
                cache_cap = new_cap;
            }
            cache[cache_len++] = ord;
        }
    }

    return cache[n - 1];
}
