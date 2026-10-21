/*
 * A249330  Primes which do not divide any repunit number R(p) = (10^p - 1)/9
 *          with p prime.
 * https://oeis.org/A249330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A249330: primes which do not divide any repunit number R(p) = (10^p‑1)/9
   with p prime.  The function a(n) returns the n‑th such prime (n ≥ 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"    /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/is_prime_small.h"  /* bool is_prime_small(seq_t n) */

/* Cache of computed sequence values. */
static seq_t *terms      = NULL;
static size_t term_len   = 0;   /* number of computed terms */
static size_t term_cap   = 0;   /* allocated capacity */

/* Simple prime generator for primes up to ~10^6 (good for 10 000 terms). */
static uint32_t *prime_cache = NULL;
static size_t prime_cnt      = 0;
static size_t prime_cap      = 0;
static uint32_t next_candidate = 2;

/* Get the next prime >= 2, expanding prime_cache as needed. */
static uint32_t next_prime(void)
{
    while (true) {
        uint32_t cand = next_candidate++;
        if (cand < 2) continue;
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_cache[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (!isprime) continue;

        if (prime_cnt == prime_cap) {
            prime_cap = prime_cap ? prime_cap * 2 : 64;
            prime_cache = (uint32_t *)realloc(prime_cache, prime_cap * sizeof(uint32_t));
        }
        prime_cache[prime_cnt++] = cand;
        return cand;
    }
}

/* Append a value to the terms cache, expanding it if necessary. */
static void add_term(seq_t v)
{
    if (term_cap == 0) {
        term_cap = 64;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
    }
    if (term_len == term_cap) {
        term_cap *= 2;
        terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    }
    terms[term_len++] = v;
}

/* Ensure that at least n terms have been computed (memoised). */
static void ensure_terms(idx_t n)
{
    while (term_len < (size_t)n) {
        uint32_t p = next_prime();

        /* Handle the special small primes. */
        if (p == 3) continue;          /* 3 divides R_3, so it is excluded */
        if (p == 2 || p == 5) {         /* 10 is 0 mod 2 or 5 → never divides a repunit */
            add_term((seq_t)p);
            continue;
        }

        /* Compute the multiplicative order of 10 modulo p. */
        idx_t t     = (idx_t)p - 1;     /* order divides p‑1 */
        idx_t order = t;
        idx_t temp  = t;

        for (idx_t q = 2; q * q <= temp; ++q) {
            if (temp % q == 0) {
                while (temp % q == 0) temp /= q;
                while (order % q == 0 &&
                       mod_pow((seq_t)10, order / q, (seq_t)p) == (seq_t)1)
                    order /= q;
            }
        }
        if (temp > 1) {                  /* remaining prime factor */
            idx_t q = temp;
            while (order % q == 0 &&
                   mod_pow((seq_t)10, order / q, (seq_t)p) == (seq_t)1)
                order /= q;
        }

        /* Include the prime if the order is composite (or 1). */
        if (!is_prime_small((seq_t)order))
            add_term((seq_t)p);
    }
}

/* Public function: returns the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defensive guard, undefined for n < 1 */
    ensure_terms(n);
    return terms[n - 1];
}
