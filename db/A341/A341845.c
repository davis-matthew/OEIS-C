/*
 * A341845  a(n) = A061026(2n): smallest k such that 2n divides phi(k), phi =
 *          A000010.
 * https://oeis.org/A341845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1860, u32 n <= 9155, u64 n <= 9155,
 *            u128 n <= 9155, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = smallest k such that 2*n divides φ(k). 
   The implementation uses a naive search over k, computing φ(k) by
   prime factorisation.  Caching of prime factorisation results and
   previous a(n) values is used to avoid recomputation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Ensure a list of primes up to 1,000,000 is available. */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;

static void ensure_primes(void) {
    if (prime_cnt > 0) return;
    const size_t LIMIT = 1000000;           /* sufficient for factorisation */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();                 /* allocation failure */
    for (size_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }
    /* Count primes */
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++prime_cnt;
    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) primes[idx++] = (uint32_t)i;
    free(is_prime);
}

/* Compute φ(n) for a 64‑bit integer n, returning a seq_t result. */
static seq_t phi_uint64(uint64_t n) {
    seq_t result = (seq_t)n;
    uint64_t rem = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= rem; ++i) {
        uint32_t p = primes[i];
        if (rem % p == 0) {
            while (rem % p == 0) rem /= p;
            result = result / (seq_t)p * (seq_t)(p - 1);
        }
    }
    if (rem > 1) {
        result = result / (seq_t)rem * (seq_t)(rem - 1);
    }
    return result;
}

/* Cache for previously computed a(n) values. */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;

static void grow_a_cache(size_t need) {
    size_t new_size = a_cache_size ? a_cache_size * 2 : 16;
    while (new_size < need) new_size *= 2;
    seq_t *new_cache = (seq_t *)realloc(a_cache, (new_size + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    /* Initialise new entries to 0 (sentinel for "not computed") */
    for (size_t i = a_cache_size + 1; i <= new_size; ++i) new_cache[i] = 0;
    a_cache = new_cache;
    a_cache_size = new_size;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* n is 1‑based; 0 is unused */

    /* Ensure primes are ready. */
    ensure_primes();

    /* Expand cache if necessary and use previously computed value. */
    if ((size_t)n > a_cache_size) grow_a_cache((size_t)n);
    if (a_cache[(size_t)n] != 0) return a_cache[(size_t)n];

    seq_t target = (seq_t)2 * (seq_t)n;   /* 2*n */

    /* Search for the smallest k with φ(k) divisible by 2*n. */
    for (uint64_t k = 2; ; ++k) {
        seq_t phi_k = phi_uint64(k);
        if (phi_k % target == 0) {
            a_cache[(size_t)n] = (seq_t)k;
            return (seq_t)k;
        }
    }
}
