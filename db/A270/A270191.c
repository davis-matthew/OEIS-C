/*
 * A270191  Numbers n for which (prime(n+1)-prime(n)) mod 3 = 1.
 * https://oeis.org/A270191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n for which (prime(n+1)-prime(n)) mod 3 = 1
 * (OEIS A270191).  The function generates primes incrementally
 * and records the indices n for which the prime gap satisfies the
 * congruence condition.  A minimal dynamic sieve (trial division
 * using already generated primes) is used so that the function
 * can be called in arbitrary order for n up to several thousands
 * in well under the time limit.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* // We only need stdbool.h for `bool`.  No other utilities are required. */

static uint64_t *primes      = NULL;   /* Array of generated primes          */
static size_t    primes_cnt  = 0;      /* Number of primes stored            */
static size_t    primes_cap  = 0;      /* Current capacity of the prime array */
static idx_t    *qual_idx    = NULL;   /* Indices n with (prime(n+1)-prime(n)) mod 3 == 1 */
static size_t    qual_cnt    = 0;      /* Number of qualifying indices       */
static size_t    qual_cap    = 0;      /* Capacity of qual_idx array         */
static uint64_t  candidate   = 2;      /* Next number to test for primality  */
static uint64_t  last_prime  = 0;      /* Value of the last generated prime  */

/* Ensure the primes array has room for at least `need` elements. */
static void ensure_primes_capacity(size_t need)
{
    if (need <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) { /* Allocation failed – terminate the program.  In the
                     context of the OEIS evaluation harness a failure
                     will be reported as a runtime error. */
        exit(EXIT_FAILURE);
    }
    primes = tmp;
    primes_cap = new_cap;
}

/* Ensure the qual_idx array has room for at least `need` elements. */
static void ensure_qual_capacity(size_t need)
{
    if (need <= qual_cap) return;
    size_t new_cap = qual_cap ? qual_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(qual_idx, new_cap * sizeof(idx_t));
    if (!tmp) { exit(EXIT_FAILURE); }
    qual_idx = tmp;
    qual_cap = new_cap;
}

/* Simple deterministic primality test using trial division with the
   primes we have already generated.  This function works for all
   values fitting in uint64_t and does not rely on any external
   utility functions. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Validate that n is at least 1.  The OEIS specification says
       that behavior for n < 1 is irrelevant. */
    if (n < 1) return 0;

    /* Ensure we have computed at least `n` qualifying indices. */
    while (qual_cnt < (size_t)n) {
        /* Find the next prime. */
        while (!is_prime_uint64(candidate)) {
            ++candidate;
        }

        /* candidate is now a new prime.  If we already have at least
           one prime, compute the gap with the previous prime and
           record the index if the gap ≡ 1 (mod 3). */
        if (primes_cnt > 0) {
            uint64_t gap = candidate - last_prime;
            if ((gap % 3ULL) == 1ULL) {
                /* The index of the lower prime is exactly `primes_cnt`
                   (primes_cnt is 1‑based before we add the new prime). */
                ensure_qual_capacity(qual_cnt + 1);
                qual_idx[qual_cnt++] = (idx_t)primes_cnt;
            }
        }

        /* Store the new prime and update bookkeeping. */
        ensure_primes_capacity(primes_cnt + 1);
        primes[primes_cnt++] = candidate;
        last_prime = candidate;
        ++candidate;           /* Move to the next number to test. */
    }

    /* Return the requested term, converting to `seq_t`. */
    return (seq_t)qual_idx[n - 1];
}
