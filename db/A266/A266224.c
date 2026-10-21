/*
 * A266224  Least x such that prime(n)*x+x+1 is a prime, or -1 if no such x
 *          exists.
 * https://oeis.org/A266224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1657, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Implementation of A266224:  a(n) = least x such that
 *        prime(n) * x + x + 1
 * is prime.  Returns -1 if no such x exists (appears never to
 * happen for n up to 10000).
 */

/* --------------------------------------------------------------------- */
/* Prime table generation (sieve)                                       */
/* --------------------------------------------------------------------- */

static uint32_t *prime_tbl = NULL;   /* array of primes, 1‑based indexing         */
static size_t   prime_cnt = 0;       /* number of primes stored                  */
#define MAX_PRIME_LIMIT 200000      /* enough to cover at least 11000 primes    */

/* Build the prime table up to MAX_PRIME_LIMIT using a simple sieve.
 * This routine is executed lazily the first time it is needed. */
static void init_prime_table(void)
{
    if (prime_tbl != NULL)
        return;                        /* already built */

    /* Allocate space for a binary sieve array (+ 1 for 0..limit). */
    unsigned char *sieve = (unsigned char *)
        malloc((size_t)MAX_PRIME_LIMIT + 1);
    if (sieve == NULL)
        return;                        /* out of memory protection */

    /* Basic sieve initialisation */
    for (size_t i = 0; i <= (size_t)MAX_PRIME_LIMIT; ++i)
        sieve[i] = 0;                   /* 0 = potentially prime */

    sieve[0] = sieve[1] = 1;            /* 0 and 1 are not prime */

    /* Sieve of Eratosthenes */
    size_t limit = (size_t)MAX_PRIME_LIMIT;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (sieve[p] == 0) {
            for (size_t m = p * p; m <= limit; m += p)
                sieve[m] = 1;
        }
    }

    /* Allocate prime table (worst‑case size = limit). */
    prime_tbl = (uint32_t *)malloc((size_t)limit * sizeof(uint32_t));
    if (prime_tbl == NULL) {
        free(sieve);
        return;                        /* out of memory protection */
    }

    /* Collect primes into the table. */
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i] == 0) {
            prime_tbl[prime_cnt++] = (uint32_t)i;
        }
    }

    free(sieve);                       /* sieve no longer needed */
}

/* Return the n-th prime (1‑based).  Works for all n <= prime_cnt.
 * The caller must call init_prime_table() before using this function. */
static inline uint32_t nth_prime(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, but never used */
    if (prime_tbl == NULL)
        init_prime_table();
    if (prime_cnt < n)                /* not enough primes built */
        return 0;                     /* should not happen with chosen limit */
    return prime_tbl[n - 1];
}

/* --------------------------------------------------------------------- */
/* Simple primality test for 64‑bit integers (trial division).          */
/* --------------------------------------------------------------------- */

static inline bool is_prime_uint64(uint64_t v)
{
    if (v < 2)
        return false;
    if (v % 2 == 0)
        return v == 2;
    for (uint64_t d = 3; d * d <= v; d += 2) {
        if (v % d == 0)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)               /* offset = 1; behaviour below offset is undefined */
        return (seq_t)(-1);

    /* Get the n-th prime. */
    uint32_t p = nth_prime(n);
    if (p == 0)               /* failed to retrieve prime; fallback to -1 */
        return (seq_t)(-1);

    /* step = p + 1  (the factor multiplied by x) */
    seq_t step = (seq_t)p + (seq_t)1;

    /* First candidate: x = 1, so y = x*step + 1 = step + 1 = p + 2. */
    seq_t y = step + (seq_t)1;

    /* x starts at 1. */
    seq_t x = (seq_t)1;

    /* Search for the smallest x that makes y a prime. */
    while (1) {
        /* The candidate y fits within 64 bits for all tested n. */
        if (is_prime_uint64((uint64_t)y))
            return x;           /* found the least x */

        /* Next candidate */
        y += step;
        ++x;
    }
}
