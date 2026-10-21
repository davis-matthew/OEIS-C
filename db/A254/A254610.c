/*
 * A254610  Number of decompositions of 2n into sums of two primes p1 <= p2
 *          such that the smallest |k*p1-p2| = 2^m+b, where |b|<=2.
 * https://oeis.org/A254610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_pow2_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of OEIS sequence A254610:
   a(n) = number of prime pairs (p1,p2) with p1 + p2 = 2n, p1 <= p2,
   such that the minimal value of |k*p1 - p2| (k >= 1) equals 2^m + b
   for some integer m >= 0 and |b| <= 2.
   The function is computed by enumerating the possible p1 <= n,
   checking that p2 is prime, computing the minimal distance, and
   testing the distance condition against the nearest powers of two.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/max_pow2_le.h"

/* ---------- Prime list management ---------- */
static uint64_t *prime_list   = NULL;   /* sorted list of primes */
static size_t   prime_count   = 0;      /* number of primes in list   */
static uint64_t prime_limit   = 0;      /* maximum prime in list */

/* Ensure that all primes up to 'limit' are available. */
static void
ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;          /* already sufficient */

    /* Allocate a simple sieve up to 'limit'. */
    size_t sz = (size_t)(limit + 1);
    unsigned char *sieve = (unsigned char *)malloc(sz);
    if (!sieve) abort();
    memset(sieve, 1, sz);
    sieve[0] = sieve[1] = 0;

    /* Sieve of Eratosthenes. */
    uint64_t sqrt_limit = (uint64_t)(sqrt((long double)limit) + 1.0);
    for (uint64_t i = 2; i <= sqrt_limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes and build the list. */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) new_list[idx++] = (uint64_t)i;

    free(prime_list);
    prime_list   = new_list;
    prime_count  = cnt;
    prime_limit  = limit;

    free(sieve);
}

/* ---------- Primality test using the prime list ---------- */
static bool
is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                     /* n >= 1 by OEIS */

    uint64_t un = (uint64_t)n;
    uint64_t two_n = un * 2;                  /* 2n */

    /* Prime list must cover all primes up to 2n. */
    ensure_primes(two_n);

    seq_t count = 0;                          /* result */

    /* Enumerate all primes p1 <= n. */
    for (size_t ip = 0; ip < prime_count; ++ip) {
        uint64_t p1 = prime_list[ip];
        if (p1 > un) break;                  /* ensure p1 <= p2 */

        uint64_t p2 = two_n - p1;            /* candidate p2 */
        if (!is_prime_uint64(p2)) continue;  /* both must be prime */

        /* Minimal distance |k*p1 - p2| over k >= 1. */
        uint64_t rem = p2 % p1;              /* remainder of division */
        uint64_t dist = rem;
        if (p1 > rem) {
            uint64_t alt = p1 - rem;
            if (alt < dist) dist = alt;
        }

        /* Check distance condition. */
        bool ok = false;
        if (dist == 0) {
            /* 0 = 2^0 - 1 : |b| = 1 <= 2 */
            ok = true;
        } else {
            uint64_t pow_low = max_pow2_le(dist);
            /* Check floor power. */
            if (dist - pow_low <= 2) ok = true;
            /* Check ceil power if still not ok. */
            if (!ok) {
                uint64_t pow_high = pow_low << 1;
                if (pow_high - dist <= 2) ok = true;
            }
        }

        if (ok) ++count;
    }

    return count;
}
