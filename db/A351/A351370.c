/*
 * A351370  a(n) = Sum_{p|n, p prime} p^prime(p).
 * https://oeis.org/A351370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = Σ_{p|n, p prime} p^prime(p)
    where prime(p) = the p‑th prime number.
    The implementation uses a precomputed list of primes and
    the helper big_pow for arbitrary‑precision exponentiation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* ------------------------------------------------------------------ */
/* Sieve parameters – enough primes to handle all primes up to
 * 100 000 (the 10 000th prime is 104 729).  This covers prime(p)
 * for any prime p ≤ 10 000.                                     */

#define SIEVE_LIMIT 200000          /* inclusive */
#define MAX_PRIMES   20000          /* ≈ number of primes ≤ SIEVE_LIMIT */

/* ------------------------------------------------------------------ */
/* Static data used by the sieve and the prime index table. */ 

static bool        is_prime[SIEVE_LIMIT + 1];
static uint64_t    primes[MAX_PRIMES];
static uint32_t    prime_index[SIEVE_LIMIT + 1];  /* 0‑based index of a prime */
static size_t      prime_count      = 0;
static bool        primes_initialized = false;

/* ------------------------------------------------------------------ */
/* Build the sieve and fill the primes array and lookup table. */

static void
init_primes(void)
{
    if (primes_initialized)
        return;

    /* mark all numbers as prime initially */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_prime[i] = true;
    if (SIEVE_LIMIT >= 0) is_prime[0] = false;
    if (SIEVE_LIMIT >= 1) is_prime[1] = false;

    /* classic Eratosthenes sieve */
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;

    /* build the primes array and the index lookup */
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) {
            prime_index[(uint32_t)i] = (uint32_t)prime_count; /* index in primes[] */
            primes[prime_count++] = (uint64_t)i;
        }

    primes_initialized = true;
}

/* ------------------------------------------------------------------ */
/* Given a prime p (p ≤ SIEVE_LIMIT) return the p‑th prime number.
 * p is a prime itself; the p‑th prime is primes[p-1] (1‑based).   */

static inline seq_t
prime_of_prime(uint64_t p)
{
    return (seq_t)primes[p - 1];
}

/* ------------------------------------------------------------------ */
/* Main sequence function */

seq_t
a(idx_t n)
{
    init_primes();

    if (n <= 1)
        return (seq_t)0;

    seq_t sum = 0;                  /* accumulator in seq_t */
    uint64_t tmp = (uint64_t)n;     /* work copy for factorisation */

    /* trial division by the precomputed primes */
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            /* strip all multiplicities of this prime */
            while (tmp % p == 0)
                tmp /= p;

            seq_t exp   = prime_of_prime(p);
            seq_t term  = big_pow((seq_t)p, exp);
            sum += term;
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (tmp > 1) {
        uint64_t p = tmp;                 /* remaining prime */
        seq_t exp  = prime_of_prime(p);
        seq_t term = big_pow((seq_t)p, exp);
        sum += term;
    }

    return sum;
}
