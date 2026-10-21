/*
 * A278022  Squares s such that s + 2 and s - 2 are semiprime.
 * https://oeis.org/A278022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 3615, u64 n <= 5200,
 *            u128 n <= 5200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A278022:
 *  Squares s such that s+2 and s-2 are semiprime.
 *  a(n) returns the n‑th such square (1‑based indexing).
 *  The implementation searches successive integers k, tests k²±2
 *  for semiprimality, and caches found squares.  The prime list
 *  is built once up to a fixed limit sufficient for n≤100 000,
 *  which covers all terms requested by the tests.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------  Prime sieve  --------- */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0, prime_cap = 0;
#define PRIME_LIMIT 1000000  /* > sqrt(10¹²), enough for n≈10⁴ */

/* Build a list of primes up to PRIME_LIMIT.  Result is cached. */
static void build_prime_list(void) {
    if (prime_list != NULL)
        return;  /* already built */

    size_t limit = PRIME_LIMIT;
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime)
        abort();

    for (size_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Estimate number of primes ≈ limit / log(limit) */
    size_t est = limit / 10;
    prime_list = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!prime_list)
        abort();
    prime_cap = est;

    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint32_t));
                if (!tmp)
                    abort();
                prime_list = tmp;
            }
            prime_list[prime_count++] = (uint32_t)i;
        }
    }
    free(is_prime);
}

/* ---------  Semiprime test (uint64_t) --------- */
static bool is_semiprime_uint64(uint64_t n) {
    if (n < 6)                 /* smallest semiprime is 2*3 = 6 */
        return false;

    int factors = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n)
            break;
        while (n % p == 0) {
            ++factors;
            n /= p;
            if (factors > 2)
                return false;
        }
    }
    if (n > 1)
        ++factors;           /* remaining prime factor */

    return factors == 2;
}

/* ---------  Result cache  --------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Ensure cache can hold at least 'cap' elements. */
static void cache_ensure(size_t cap) {
    if (cap <= cache_cap)
        return;
    size_t new_cap = cap > 64 ? cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

static uint64_t next_k = 2;   /* next integer to test */

/* ---------  Main function  --------- */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* graceful handling of out‑of‑range input */

    /* Build prime list once. */
    build_prime_list();

    /* Extend cache until we have at least n elements. */
    while (cache_len < (size_t)n) {
        for (;;) {
            uint64_t k = next_k;
            /* Compute k² safely in 128‑bit, then cast to uint64_t. */
            unsigned __int128 prod = (unsigned __int128)k * k;
            if (prod > (unsigned __int128)UINT64_MAX)
                abort();      /* sequence exceeds 64‑bit for requested n */
            uint64_t sq = (uint64_t)prod;

            if (is_semiprime_uint64(sq - 2) && is_semiprime_uint64(sq + 2)) {
                cache_ensure(cache_len + 1);
                cache[cache_len] = (seq_t)sq;
                ++cache_len;
                break;       /* found next qualifying square */
            }
            ++next_k;       /* advance k for next trial */
        }
        ++next_k;           /* start next search from following integer */
    }

    return cache[n - 1];
}
