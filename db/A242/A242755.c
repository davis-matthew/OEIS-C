/*
 * A242755  Primes p such that pi(p)*q == 1 (mod p) for some prime q < p,
 *          where pi(p) is the number of primes not exceeding p.
 * https://oeis.org/A242755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 745, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A242755:
   primes p such that π(p) * q ≡ 1 (mod p) for some prime q < p. */

/* The algorithm streams through the primes, keeps the running count
   π(p), finds the modular inverse of π(p) modulo p, and checks whether
   that inverse is a prime < p.  All qualifying primes are cached,
   so subsequent calls return immediately for already discovered terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x); */
#include "../utils/seq_realloc.h" /* seq_t* seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

/* Compute the modular inverse of a modulo m.
   Returns true and stores the result in *inv if the inverse exists.
   All operands are unsigned but the algorithm uses int128 arithmetic
   to avoid overflow and wrap‑around. */
static bool modinv_uint64(uint64_t a, uint64_t m, uint64_t *inv)
{
    __int128_t t     = 0;
    __int128_t newt  = 1;
    __int128_t r     = m;
    __int128_t newr  = a;
    while (newr != 0) {
        uint64_t q = (uint64_t)(r / newr);
        __int128_t tmp = t - (__int128_t)q * newt;
        t     = newt;
        newt  = tmp;
        tmp   = r - (__int128_t)q * newr;
        r     = newr;
        newr  = tmp;
    }
    if (r > 1) return false;          /* no inverse (m and a not coprime) */
    if (t < 0) t += m;
    *inv = (uint64_t)t;
    return true;
}

/* Static state for incremental construction and memoisation. */
static seq_t *qual_primes = NULL;   /* cached qualifying primes */
static size_t qual_cap   = 0;       /* current capacity of qual_primes */
static size_t qual_cnt   = 0;       /* number of qualifying primes stored */

static uint64_t cur_candidate = 2;  /* next integer to test for primality */
static uint64_t pi_val = 0;         /* π(p) for the last examined prime */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for index 0 */

    /* Expand the cache until it contains at least n terms. */
    while (qual_cnt < n) {
        /* Find the next prime. */
        while (!is_prime(cur_candidate)) {
            cur_candidate++;
        }

        /* We have a prime p = cur_candidate. */
        pi_val++;                     /* increment π(p) */
        const uint64_t p = cur_candidate;
        const uint64_t pi = pi_val;

        /* Compute modular inverse of π(p) modulo p. */
        uint64_t inv;
        if (modinv_uint64(pi, p, &inv)) {
            /* inv is in [0, p-1].  It represents the unique q such
               that π(p)*q ≡ 1 (mod p).  Check that it is a prime < p. */
            if (inv >= 2 && is_prime(inv)) {
                /* Grow the array if necessary. */
                if (qual_cnt >= qual_cap) {
                    size_t new_cap = qual_cap ? qual_cap * 2 : 8;
                    seq_t *tmp = seq_realloc(qual_primes, qual_cap, new_cap);
                    if (!tmp) return (seq_t)0;  /* out‑of‑memory fallback */
                    qual_primes = tmp;
                    qual_cap = new_cap;
                }
                qual_primes[qual_cnt++] = (seq_t)p;
            }
        }

        cur_candidate++;  /* move to the next integer for the next iteration */
    }

    return qual_primes[n - 1];      /* zero‑based index into the cache */
}
