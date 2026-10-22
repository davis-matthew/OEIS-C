/*
 * A356448  Even numbers k such that k^2 is in A014567.
 * https://oeis.org/A356448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1257, u64 n <= 7127,
 *            u128 n <= 7127, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A356448:
   a(n) = the nth even number k such that gcd(k, sigma(k^2)) = 1,
   where sigma is the sum of divisors function. */
/* The routine generates the sequence lazily, caching previously found
   terms.  Each term is obtained by testing successive even integers
   and accepting those for which gcd(k, sigma(k^2)) == 1. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* array of computed terms          */
static size_t cache_len = 0;         /* number of terms stored           */
static size_t cache_cap = 0;         /* capacity of the array            */

static uint32_t *prime_list = NULL;  /* sieve of primes up to 1e6        */
static size_t prime_cnt = 0;         /* number of primes                 */

static uint64_t next_even = 2;       /* next candidate even number       */

/* ensure that we have a list of primes up to 1e6  */
static void init_primes(void) {
    if (prime_list) return;
    const uint32_t limit = 1000000;
    uint8_t *sieve = (uint8_t *)calloc(limit + 1, 1);
    sieve[0] = sieve[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i)
        if (!sieve[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++cnt;
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!sieve[i]) prime_list[idx++] = i;
    prime_cnt = cnt;
    free(sieve);
}

/* grow the cache array to hold at least 'needed' elements */
static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/* factor k and compute sigma(k^2) */
static seq_t sigma_of_k_square(uint64_t k) {
    uint64_t n = k;
    seq_t sigma = 1;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) { n /= p; ++e; }
            /* term = (p^(2e+1) - 1) / (p - 1) */
            seq_t pe = 1;
            unsigned exp = 2 * e + 1;
            for (unsigned j = 0; j < exp; ++j) pe *= (seq_t)p;
            seq_t term = (pe - 1) / (seq_t)(p - 1);
            sigma *= term;
        }
    }
    if (n > 1) {                    /* remaining prime factor */
        uint32_t p = (uint32_t)n;    /* exponent in k is 1 */
        seq_t pe = (seq_t)p * p * p; /* p^3 */
        seq_t term = (pe - 1) / (seq_t)(p - 1);
        sigma *= term;
    }
    return sigma;
}

/* return the nth term of A356448 (indexing starts at 1) */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, but safety */
    init_primes();
    size_t target = (size_t)n;
    if (target > cache_len) {
        for (size_t needed = cache_len + 1; needed <= target; ++needed) {
            while (true) {
                uint64_t cand = next_even;
                next_even += 2;
                seq_t sigma = sigma_of_k_square(cand);
                if (gcd((seq_t)cand, sigma) == 1) {
                    ensure_cache_capacity(cache_len + 1);
                    cache[cache_len++] = cand;
                    break;
                }
            }
        }
    }
    return cache[target - 1];
}
