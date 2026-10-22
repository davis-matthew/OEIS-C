/*
 * A307782  Even integers k such that there exists a prime p with p=min{q: q
 *          prime and (k-q) prime} and k < p^3.
 * https://oeis.org/A307782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307782:
 * Even integers k such that the smallest Goldbach prime p satisfies k < p^3.
 * a(n) is the n-th such even integer (1‑based). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Prime generation (simple sieve) ---------- */
static const idx_t PRIME_LIMIT = 100000;
static uint8_t *sieve_base = NULL;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    sieve_base = (uint8_t*)calloc(PRIME_LIMIT + 1, sizeof(uint8_t));
    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!sieve_base[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i) {
                sieve_base[j] = 1;
            }
        }
    }
    prime_cap = PRIME_LIMIT / 10 + 10;
    primes = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve_base[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                primes = (uint64_t*)realloc(primes, prime_cap * sizeof(uint64_t));
            }
            primes[prime_cnt++] = (uint64_t)i;
        }
    }
    primes_initialized = true;
}

/* ---------- Integer primality test (trial division) ---------- */
static bool is_prime_simple(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t current_even = 2;          /* will become 4 on first loop */

    init_primes();

    while (cache_size < n) {
        current_even += 2;                 /* next even integer */
        seq_t k = (seq_t)current_even;

        uint64_t p = 0;
        idx_t half = current_even / 2;
        for (size_t i = 0; i < prime_cnt && primes[i] <= half; ++i) {
            if (is_prime_simple((uint64_t)k - primes[i])) {
                p = primes[i];
                break;
            }
        }
        if (p == 0) continue;             /* should not happen for k>=4 */

        seq_t p3 = (seq_t)p * (seq_t)p * (seq_t)p;
        if ((seq_t)k < p3) {
            if (cache_size == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = k;
        }
    }
    return cache[n-1];
}
