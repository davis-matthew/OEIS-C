/*
 * A308268  Numbers k such that 1 + A045763(k) is prime.
 * https://oeis.org/A308268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 6995, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308268: k such that k-φ(k)-τ(k) is prime (OEIS). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

#define PRIME_SIEVE_LIMIT 4000000  /* sufficient for n <= 10000 */

static bool primes_ready = false;
static size_t prime_cnt = 0;
static uint32_t *primes = NULL;

static void init_primes(void)
{
    if (primes_ready) return;

    unsigned char *sieve = (unsigned char*)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!sieve) abort();
    memset(sieve, 1, PRIME_SIEVE_LIMIT + 1);
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[j] = 0;

    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i]) ++prime_cnt;

    primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i]) primes[idx++] = (uint32_t)i;

    free(sieve);
    primes_ready = true;
}

static seq_t *cache = NULL;
static size_t cache_sz = 0, cache_cap = 0;

static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *new_mem = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_mem) abort();
    cache = new_mem;
    cache_cap = new_cap;
}

static void phi_tau(uint64_t k, uint64_t *phi_out, uint64_t *tau_out)
{
    uint64_t n = k;
    uint64_t phi = k;
    uint64_t tau = 1;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            phi = phi / p * (p - 1);
            tau *= (uint64_t)(exp + 1);
        }
    }
    if (n > 1) {  /* remaining prime factor */
        phi = phi / n * (n - 1);
        tau *= 2;
    }
    *phi_out = phi;
    *tau_out = tau;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    init_primes();

    static uint64_t current_k = 10;

    while (cache_sz < (size_t)n) {
        uint64_t k = current_k++;
        uint64_t phi, tau;
        phi_tau(k, &phi, &tau);

        uint64_t expr = k - phi - tau;
        if (expr >= 2 && is_prime_big((seq_t)expr)) {
            if (cache_sz == cache_cap) grow_cache((cache_sz < 16) ? 16 : cache_sz * 2);
            cache[cache_sz++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
