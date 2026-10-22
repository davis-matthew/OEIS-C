/*
 * A338316  Odd numbers whose distinct prime indices are pairwise coprime,
 *          where a singleton is always considered coprime.
 * https://oeis.org/A338316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a sieve-based search for odd numbers whose distinct prime
   indices are pairwise coprime. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Sieve of Eratosthenes – all primes up to LIMIT and corresponding
 *  prime indices  (prime_idx[p] = position of prime p, 1‑based).  */
#define SIEVE_LIMIT 5000000            /* chosen large enough for n up to 10 000 */

static uint32_t *prime_idx   = NULL;   /* prime_idx[p] = index of prime p (1‑based)   */
static uint32_t *prime_list  = NULL;   /* list of primes up to SIEVE_LIMIT          */
static size_t    prime_cnt   = 0;      /* number of primes found                    */
static bool      sieve_ready = false;

/* Build the sieve once (static init).  */
static void build_sieve(void)
{
    if (sieve_ready) return;
    sieve_ready = true;

    /* Allocate arrays.  */
    uint8_t *is_prime = (uint8_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint8_t));
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Rough upper bound on number of primes ≤ SIEVE_LIMIT is limit/log(limit). */
    size_t prime_cap = SIEVE_LIMIT / 10;
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    prime_idx  = (uint32_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) prime_idx[i] = 0;

    /* Classic sieve – store primes and their indices. */
    for (size_t p = 2; p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            if (prime_cnt == prime_cap) {
                /* grow the prime list if needed (should not happen). */
                prime_cap <<= 1;
                prime_list = (uint32_t *)realloc(prime_list,
                                                 prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt] = (uint32_t)p;
            prime_idx[p] = (uint32_t)(prime_cnt + 1);   /* 1‑based index */
            ++prime_cnt;

            if (p * p <= SIEVE_LIMIT) {
                for (size_t m = p * p; m <= SIEVE_LIMIT; m += p)
                    is_prime[m] = 0;
            }
        }
    }

    free(is_prime);
}

/* --------------------------------------------------------------------- */
/* Simple Euclidean GCD for 32‑bit values */
static uint32_t gcd32(uint32_t a, uint32_t b)
{
    while (b != 0) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --------------------------------------------------------------------- */
/* Test whether an odd integer n has pairwise coprime distinct prime
   indices.  */
static bool good_candidate(uint64_t n)
{
    if (n == 1) return true;

    uint64_t m = n;
    uint32_t indices[32];
    size_t  idx_cnt = 0;

    /* Factor using precomputed primes. */
    for (size_t i = 0; i < prime_cnt &&
                     (uint64_t)prime_list[i] * prime_list[i] <= m; ++i) {
        uint32_t p = prime_list[i];
        if (m % p == 0) {
            uint32_t ip = prime_idx[p];
            for (size_t j = 0; j < idx_cnt; ++j) {
                if (gcd32(ip, indices[j]) != 1)
                    return false;
            }
            indices[idx_cnt++] = ip;
            while (m % p == 0) m /= p;
        }
    }

    /* Remaining prime factor > sqrt(m). */
    if (m > 1) {
        uint32_t ip;
        if (m <= (uint64_t)SIEVE_LIMIT) {
            ip = prime_idx[(uint32_t)m];
        } else {
            /* m is a prime > SIEVE_LIMIT – determine its index by binary
               search in the prime list (should not occur with the chosen
               sieve limit for n ≤ 10 000). */
            size_t lo = 0, hi = prime_cnt;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (prime_list[mid] <= m) lo = mid + 1;
                else hi = mid;
            }
            ip = (uint32_t)lo;   /* number of primes ≤ m  */
        }
        for (size_t j = 0; j < idx_cnt; ++j) {
            if (gcd32(ip, indices[j]) != 1)
                return false;
        }
    }

    return true;
}

/* --------------------------------------------------------------------- */
/* Public sequence function.  */
seq_t a(idx_t n)
{
    build_sieve();

    if (n == 1) return (seq_t)1;

    uint64_t count = 1;   /* count of valid odd numbers seen (including 1) */
    uint64_t cand  = 3;   /* start testing from the first odd > 1 */

    while (true) {
        if (good_candidate(cand)) {
            ++count;
            if (count == (uint64_t)n)
                return (seq_t)cand;
        }
        cand += 2;   /* next odd candidate */
    }
}
