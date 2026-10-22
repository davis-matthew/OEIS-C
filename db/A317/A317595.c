/*
 * A317595  a(n) is the number of primes between 2n and the largest prime p
 *          such that 2n-p is also a prime.
 * https://oeis.org/A317595
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes q with p < q < 2n, where p is the
 *            largest prime ≤ 2n-2 such that 2n-p is prime
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static sieve state.  Not re‑entrant but satisfies “any order of calls”. */
static uint8_t *is_prime = NULL;   /* 1 if index is prime, 0 otherwise */
static uint64_t *primes  = NULL;   /* list of primes up to current limit */
static size_t    prime_cnt = 0;     /* number of primes in the list          */
static idx_t     sieve_limit = 0;   /* maximum number that has been sieved   */

/* Rebuild the sieve up to at least new_limit. */
static void ensure_limit(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;                      /* already big enough */

    /* Free old data. */
    free(is_prime);
    free(primes);

    /* Allocate new sieve bitmap. */
    is_prime = (uint8_t *)malloc((size_t)(new_limit + 1) * sizeof(uint8_t));
    if (!is_prime)
        exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= new_limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = 0;
    is_prime[1] = 0;

    /* Sieve of Eratosthenes.  Avoid overflow by testing p <= new_limit/p. */
    for (idx_t p = 2; p <= new_limit / p; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= new_limit; m += p)
                is_prime[m] = 0;
        }
    }

    /* Count primes to allocate prime list. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (is_prime[i])
            ++cnt;

    /* Build prime list. */
    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes)
        exit(EXIT_FAILURE);
    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (is_prime[i])
            primes[pos++] = (uint64_t)i;
    }
    prime_cnt = cnt;
    sieve_limit = new_limit;
}

/* Return the largest prime p ≤ 2n-2 such that 2n-p is prime. */
static uint64_t largest_goldbach_prime(idx_t S)
{
    /* Scan primes backwards for speed. */
    for (size_t idx = prime_cnt; idx > 0; --idx) {
        uint64_t p = primes[idx - 1];
        if (p > S - 2)          /* ensures 2n-p ≥ 2 */
            continue;
        if (is_prime[(idx_t)(S - p)])
            return p;
    }
    return 2; /* fallback, should not be reached for n ≥ 2 */
}

/* Count primes strictly between lower (exclusive) and upper (exclusive). */
static seq_t count_primes_between(uint64_t lower, idx_t upper)
{
    seq_t cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = primes[i];
        if (q <= lower)
            continue;
        if (q >= upper)
            break;
        ++cnt;
    }
    return cnt;
}

/* Main sequence function.  Handles any order of n. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                  /* undefined below OEIS offset */

    idx_t S = 2 * n;                /* upper bound for Goldbach pairs */
    ensure_limit(S);

    uint64_t p = largest_goldbach_prime(S);   /* largest Goldbach prime */
    return count_primes_between(p, S);
}
