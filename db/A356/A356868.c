/*
 * A356868  a(n) = n^2 * prime(n).
 * https://oeis.org/A356868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * prime(n) (prime(1) = 2)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Thread‐local static cache of the first prime_cnt primes. */
static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;  /* number of primes stored */
static size_t prime_cap   = 0;  /* capacity of prime_cache   */
static uint64_t next_pri   = 2; /* next integer to test for primality */

/* Grow the prime_cache to hold at least 'need' primes. */
static void grow_prime_cache(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();          /* allocation failure – hard stop */
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* Test if x is prime using the primes already in prime_cache. */
static bool is_prime_trial(uint64_t x)
{
    if (x < 2) return false;
    /* Check divisibility by stored primes up to sqrt(x). */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > x / p) break;      /* p*p would overflow or exceed x */
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that at least 'n' primes are in prime_cache. */
static void ensure_n_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;
    grow_prime_cache((size_t)n);
    while (prime_cnt < (size_t)n) {
        if (is_prime_trial(next_pri)) {
            prime_cache[prime_cnt++] = next_pri;
        }
        ++next_pri;
    }
}

/* Main function requested. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* OEIS offset is 1; undefined for 0 */

    /* Fill cache up to the n‑th prime. */
    ensure_n_primes(n);
    uint64_t p = prime_cache[(size_t)n - 1];

    /* Compute n^2 * p in seq_t to avoid intermediate overflows. */
    seq_t s = (seq_t)n;
    seq_t sq = s * s;
    return sq * (seq_t)p;
}
