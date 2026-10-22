/*
 * A342068  a(n) is the smallest k > 1 such that there are more primes in the
 *          interval [(k-1)*n + 1, k*n] than there are in the interval
 *          [(k-2)*n + 1, (k-1)*n].
 * https://oeis.org/A342068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 1 such that prime_count((k-1)*n+1 .. k*n) >
 *            prime_count((k-2)*n+1 .. (k-1)*n) (prime_count(x..y) = π(y) -
 *            π(x-1), with π(0)=0)
 * Range:     u8 n <= 1865, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* uint64_t */

/* Prime count cache ------------------------------------------------------------- */
static seq_t *pi_cache = NULL; /* array of π values for indices 0 .. pi_cap-1 */
static size_t pi_cap   = 0;    /* current size of pi_cache (entries stored) */

/* Ensure the sieve and prefix array are constructed up to 'limit' (inclusive) */
static void
ensure_sieve(idx_t limit)
{
    if (pi_cap > 0 && (size_t)limit + 1 <= pi_cap)
        return;                           /* already large enough */

    size_t new_cap = (size_t)limit + 1;     /* +1 for index 0 */

    /* Allocate temporary array of marks (1 = prime, 0 = composite) */
    uint8_t *is_prime = (uint8_t *)malloc(new_cap);
    if (!is_prime)
        abort();

    /* Assume all numbers >= 2 are prime initially */
    for (size_t i = 0; i < new_cap; ++i)
        is_prime[i] = 1;
    if (new_cap > 0) is_prime[0] = 0;
    if (new_cap > 1) is_prime[1] = 0;

    /* Classic Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Extend the prime‑count cache */
    seq_t *new_pi = (seq_t *)realloc(pi_cache, new_cap * sizeof(seq_t));
    if (!new_pi)
        abort();
    pi_cache = new_pi;

    /* Build the prefix prime counts */
    seq_t count = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (is_prime[i])
            ++count;
        pi_cache[i] = count;
    }

    pi_cap = new_cap;
    free(is_prime);
}

/* Return π(x) using the cache (builds it if necessary) */
static inline seq_t
pi_at(idx_t x)
{
    if (x >= pi_cap)
        ensure_sieve(x);
    return pi_cache[x];
}

/* Sequence function -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* sequence offset is 1; undefined for n < 1 */
        return 0;

    /* Search for the smallest k > 1 satisfying the required inequality. */
    for (idx_t k = 2; ; ++k) {
        /* Boundaries of the two intervals (inclusive). */
        idx_t low1  = (k - 2) * n + 1;   /* for k==2 gives 1 */
        idx_t high1 = (k - 1) * n;
        idx_t low2  = (k - 1) * n + 1;
        idx_t high2 = k * n;

        /* Ensure sieve covers the largest needed index. */
        if (high2 >= pi_cap)
            ensure_sieve(high2);

        /* Count primes in [low1, high1] and [low2, high2]. */
        seq_t primes1 = pi_at(high1) - (low1 > 1 ? pi_at(low1 - 1) : 0);
        seq_t primes2 = pi_at(high2) - pi_at(low2 - 1);

        if (primes2 > primes1)
            return k;          /* found the required k */
    }
}
