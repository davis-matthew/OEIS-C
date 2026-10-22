/*
 * A317531  Expansion of Sum_{p prime, k>=1} x^(p^k)/(1 + x^(p^k)).
 * https://oeis.org/A317531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n, d is a prime power} (-1)^{n/d+1} (equivalently
 *            add +1 if (n/d) is odd, otherwise add -1)
 * Range:     i8 n <= 8536, i16 n <= 8536, i32 n <= 8536, i64 n <= 8536,
 *            i128 n <= 8536, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool initialized = false;
static idx_t current_limit = 0;
static uint64_t *primes = NULL;          /* list of primes up to current_limit            */
static size_t primes_count = 0;
static uint64_t *prime_powers = NULL;    /* all prime powers ≤ current_limit (including primes) */
static size_t prime_powers_count = 0;

/* Ensure that all data structures are built up to at least 'limit' */
static void _ensure_limit(idx_t limit)
{
    if (limit <= current_limit) return;

    /* free old allocations if any */
    free(primes);
    free(prime_powers);

    /* Sieve of Eratosthenes up to 'limit' */
    size_t sz = (size_t)(limit + 1);
    bool *is_prime = (bool *)malloc(sz * sizeof(bool));
    if (!is_prime) abort();
    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[(size_t)j] = false;
        }
    }

    /* Count primes */
    size_t pc = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[(size_t)i]) ++pc;
    primes = (uint64_t *)malloc(pc * sizeof(uint64_t));
    if (!primes) abort();
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i]) primes[k++] = (uint64_t)i;
    primes_count = pc;

    /* Generate all prime powers ≤ limit */
    size_t cap = pc * 2 + 10;
    prime_powers = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!prime_powers) abort();
    size_t pp = 0;
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        uint64_t pow = p;
        while (pow <= limit) {
            if (pp == cap) {
                cap *= 2;
                prime_powers = (uint64_t *)realloc(prime_powers, cap * sizeof(uint64_t));
                if (!prime_powers) abort();
            }
            prime_powers[pp++] = pow;
            if (pow > limit / p) break;
            pow *= p;
        }
    }
    prime_powers_count = pp;

    free(is_prime);
    current_limit = limit;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* undefined, but safe default */
    _ensure_limit(n);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < prime_powers_count; ++i) {
        uint64_t d = prime_powers[i];
        if (d > n) break;
        if ((n % d) == 0) {
            idx_t q = n / d;          /* q = n/d */
            sum += (seq_t)((q & 1ULL) ? 1 : -1);
        }
    }
    return sum;
}
