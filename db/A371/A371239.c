/*
 * A371239  Squarefree numbers that are zeroless in base 3 (A032924).
 * https://oeis.org/A371239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1348, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_zero_free_base3_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A371239 by scanning natural numbers and keeping those that
   are squarefree and contain no zero digits in base‑3.  The function
   returns the value of a(n), with a(1)=1.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_zero_free_base3_u64.h"

/* ------------------------------------------------------------------ */
/* Prime helper – generates all primes up to LIMIT (inclusive).       */
/* ------------------------------------------------------------------ */
static const uint64_t PRIME_LIMIT = 20000;   /* > sqrt(maximal tested value)   */
static uint64_t *primes = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_count > 0) return;          /* already built      */
    bool *sieve = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!sieve) return;                   /* in case of OOM     */
    for (uint64_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i) {
                sieve[j] = false;
            }
        }
    }
    /* Count primes first */
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) if (sieve[i]) ++prime_count;
    primes = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
    if (!primes) { free(sieve); return; }
    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) primes[idx++] = i;
    }
    free(sieve);
}

/* ------------------------------------------------------------------ */
/* Test whether an unsigned 64‑bit integer has no zero digit in base‑3.*/

/* ------------------------------------------------------------------ */
/* Test whether an unsigned 64‑bit integer is squarefree.             */
/* ------------------------------------------------------------------ */
static bool is_squarefree_u64(uint64_t x)
{
    if (x <= 1) return true; /* 1 is considered squarefree       */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        uint64_t pp = p * p;            /* fits in 64‑bit for all p <= 20000   */
        if (pp > x) break;
        if ((x % pp) == 0ULL) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; callers should not use 0       */
    init_primes();

    uint64_t candidate = 1;    /* first positive integer to test            */
    idx_t      count     = 0;  /* number of valid terms found so far        */

    for (;;) {
        if (is_zero_free_base3_u64(candidate) && is_squarefree_u64(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;  /* cast to whatever seq_t is            */
            }
        }
        ++candidate;
    }
}
