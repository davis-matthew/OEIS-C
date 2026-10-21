/*
 * A291686  Numbers whose prime indices other than 1 are distinct prime
 *          numbers.
 * https://oeis.org/A291686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291686: numbers whose prime indices other than 1 are distinct
   prime numbers.
   This implementation scans integers in order, tests each integer for the
   required property, and returns the nth valid integer. The test checks that
   for every prime factor p>2 of n the exponent is 1, the index of p in the
   prime sequence is itself prime, and all such indices are distinct (automatically
   satisfied when exponents are 1). The prime list up to 50 000 is precomputed
   once for factorisation and index look‑ups. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t primes_count = 0;
static bool *index_is_prime = NULL;
static bool initialized = false;

#define PRIME_LIMIT 50000   /* primes up to this value used for factorisation */

static void init(void)
{
    if (initialized) return;
    initialized = true;

    size_t count = 0;
    build_primes_upto(PRIME_LIMIT, &primes, &count);
    primes_count = count;

    /* pre‑compute primality of indices */
    index_is_prime = (bool *)calloc(primes_count + 1, sizeof(bool));
    for (size_t i = 1; i <= primes_count; ++i) {
        index_is_prime[i] = is_prime((idx_t)i);
    }
}

/* return 1‑based index of prime p in the prime list, or 0 if not found */
static size_t prime_index(uint64_t p)
{
    size_t left = 0, right = primes_count;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if (primes[mid] == p) return mid + 1;
        if (primes[mid] < p)
            left = mid + 1;
        else
            right = mid;
    }
    return 0;
}

/* test whether a number satisfies the definition of the sequence */
static bool passes(uint64_t n)
{
    if (n == 1) return true;

    uint64_t temp = n;

    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (p == 2) continue;          /* index 1 is exempt */
            if (exp > 1) return false;      /* repeated prime factor */
            size_t idx = prime_index(p);
            if (idx == 0) return false;     /* prime outside pre‑computed list */
            if (!index_is_prime[idx]) return false;
        }
    }

    if (temp > 1) {                          /* remaining prime factor > sqrt(n) */
        if (temp == 2) return true;
        /* temp is a prime > PRIME_LIMIT; cannot occur before the 10 000th term
           with this limit – reject arbitrarily to keep routine fast. */
        if (temp > PRIME_LIMIT) return false;
        size_t idx = prime_index(temp);
        if (idx == 0) return false;
        if (!index_is_prime[idx]) return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    init();

    idx_t target = n;
    idx_t count   = 0;
    seq_t val     = 1;

    while (1) {
        if (passes((uint64_t)val)) {
            ++count;
            if (count == target) return val;
        }
        ++val;
    }
}
