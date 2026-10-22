/*
 * A306997  Number of primes q < prime(n), such that 6*prime(n) + q is prime.
 * https://oeis.org/A306997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes q < prime(n) for which 6*prime(n)+q is
 *            prime
 * Range:     u8 n <= 991, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool *prime_sieve = NULL;   /* sieve array up to LIMIT */
static idx_t *all_primes = NULL;   /* list of primes in order */
static size_t prim_cnt = 0;
static const idx_t LIMIT = 800000; /* enough for n ≤ 10000  (prime(10000)=104729) */
static bool initialized = false;

/* Build sieve and prime list once */
static void init_primes(void)
{
    if (initialized) return;

    prime_sieve = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!prime_sieve) return;          /* allocation failed */

    /* Sieve of Eratosthenes */
    for (idx_t i = 0; i <= LIMIT; ++i) prime_sieve[i] = true;
    prime_sieve[0] = prime_sieve[1] = false;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (prime_sieve[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                prime_sieve[j] = false;
        }
    }

    /* Count primes */
    prim_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (prime_sieve[i]) prim_cnt++;

    /* Build prime list */
    all_primes = (idx_t *)malloc(prim_cnt * sizeof(idx_t));
    if (!all_primes) return;

    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (prime_sieve[i]) all_primes[idx++] = i;

    initialized = true;
}

/* Sequence entry */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* off‑by‑offset; harmless */

    if (!initialized) init_primes();
    if (n == 1) return 0;   /* only one prime, no smaller q */

    idx_t p = all_primes[n - 1];   /* prime(n) */
    seq_t cnt = 0;

    /* Count suitable q < p */
    for (idx_t i = 0; i + 1 < n; ++i) {
        idx_t q = all_primes[i];
        idx_t x = 6 * p + q;           /* 6*prime(n)+q */
        if (x <= LIMIT && prime_sieve[x]) ++cnt;
    }

    return cnt;
}
