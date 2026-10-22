/*
 * A348907  If n is prime, a(n) = n, else a(n) = a(n-pi(n)), n >= 2; where pi
 *          is the prime counting function A000720.
 * https://oeis.org/A348907
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9905, u32 n <= 9905, u64 n <= 9905,
 *            u128 n <= 9905, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (recurrence): a(n) = n          if n is prime
 *                            a(n) = a(n-pi(n)) else,
 * where pi(n) is the prime‑counting function.
 * The implementation iteratively reduces n until a prime is reached,
 * using a simple sieve to answer primality and pi(n) queries on demand.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Prime sieve infrastructure                                       */
/* ------------------------------------------------------------------ */
static idx_t sieve_limit = 0;      /* maximum n currently sieved          */
static uint8_t *prime_tbl    = NULL;  /* 1 if i is prime, 0 otherwise       */
static idx_t *pi_tbl         = NULL;  /* pi(i) = number of primes <= i      */

/* Increase the sieve to cover at least limit n (no recursion). */
static void expand_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;                      /* already large enough */

    /* Allocate new tables.  Use calloc to zero-initialize. */
    size_t size = (size_t)new_limit + 1;
    uint8_t *new_prime = (uint8_t*)calloc(size, sizeof(uint8_t));
    if (!new_prime)
        return;                      /* out of memory – caller will not crash */

    /* Initially mark all numbers >=2 as possibly prime. */
    for (idx_t i = 2; i <= new_limit; ++i)
        new_prime[i] = 1;

    /* Classic Eratosthenes sieve. */
    for (idx_t i = 2; (idx_t)(i * i) <= new_limit; ++i) {
        if (new_prime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                new_prime[j] = 0;
        }
    }

    /* Create cumulative prime count array pi_tbl. */
    idx_t *new_pi = (idx_t*)calloc(size, sizeof(idx_t));
    if (!new_pi) {
        free(new_prime);
        return;
    }
    idx_t cnt = 0;
    for (idx_t i = 0; i <= new_limit; ++i) {
        if (new_prime[i])
            ++cnt;
        new_pi[i] = cnt;
    }

    /* Replace old tables. */
    free(prime_tbl);
    free(pi_tbl);
    prime_tbl = new_prime;
    pi_tbl    = new_pi;
    sieve_limit = new_limit;
}

/* Ensure the sieve covers at least n. */
static void ensure_sieve(idx_t n)
{
    if (n > sieve_limit)
        expand_sieve(n);
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                */
/* ------------------------------------------------------------------ */

/* Now we can answer a(n) for any n >= 2. */
seq_t a(idx_t n)
{
    if (n < 2)        /* undefined below the offset */
        return 0;

    /* Make sure we have pi information up to n. */
    ensure_sieve(n);

    /* Iteratively reduce n until a prime is found. */
    while (!prime_tbl[n]) {
        idx_t pi_n = pi_tbl[n];
        n -= pi_n;
        /* After reduction n never increases, so no need to grow the sieve. */
    }

    /* n is now prime; cast to seq_t for the return type. */
    return (seq_t)n;
}
