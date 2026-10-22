/*
 * A320384  Primes p such that 3/2 is a primitive root modulo p.
 * https://oeis.org/A320384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   mod_mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A320384 – primes p such that 3/2 is a primitive root modulo p.
    This function returns the n‑th such prime (a(1) = 7).  It
    iterates over primes, tests each with the standard primitive‑root
    criterion and caches all found terms for future calls.  No
    pre‑computed table is used.  The code is valid C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul64.h"

/* ------ Simple 64‑bit modular arithmetic utilities ------ */

/* 128‑bit multiplication modulo mod (works in C++ and C11). */

/* Modular exponentiation: base^exp mod mod. */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = mod_mul64(res, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* ------ Prime table management ------ */

static uint64_t *prime_list = NULL;   /* stored primes */
static size_t   prime_cnt  = 0;        /* number of stored primes */
static size_t   prime_cap  = 0;        /* array capacity */
static uint64_t next_candidate = 5;   /* next odd to test */

/* Append a prime to the table, growing the array. */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        prime_list = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Test n for primality using trial division by primes we already have. */
static bool is_prime_simple(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        if (q > n / q) break;
        if (n % q == 0) return false;
    }
    return true;
}

/* Initialise prime table with 2 and 3. */
static void init_prime_table(void)
{
    if (prime_list == NULL) {
        prime_cap = 8;
        prime_list = (uint64_t*)calloc(prime_cap, sizeof(uint64_t));
        add_prime(2);
        add_prime(3);
        next_candidate = 5;
    }
}

/* ------ Sequence term cache ------ */

static seq_t *terms = NULL;    /* array of found terms */
static size_t  terms_cnt = 0;  /* number of terms stored */
static size_t  terms_cap = 0;  /* capacity of array */

/* Append a term to the cache. */
static void add_term(seq_t v)
{
    if (terms_cnt == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 4;
        terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
    terms[terms_cnt++] = v;
}

/* Initialise the term cache. */
static void init_sequence_cache(void)
{
    if (terms == NULL) {
        terms_cap = 4;
        terms = (seq_t*)calloc(terms_cap, sizeof(seq_t));
        terms_cnt = 0;
    }
}

/* ------ Primitive‑root test for a prime p ------ */

/* Returns true if 3/2 is a primitive root modulo p. */
static bool check_prime(uint64_t p)
{
    /* Compute g = 3 * (2^{-1} mod p).  For odd p, inv2 = (p+1)/2. */
    uint64_t inv2 = (p + 1) / 2;         /* 2*inv2 ≡ 1 (mod p) */
    uint64_t g = (3 * inv2) % p;

    uint64_t phi = p - 1;
    uint64_t m   = phi;                  /* factorisation of phi */

    /* Factorise phi using the primes we already know. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        if (q > m / q) break; /* q*q > m */
        if (m % q == 0) {
            /* Remove all powers of q from m. */
            do { m /= q; } while (m % q == 0);
            /* If g^(phi/q) ≡ 1 (mod p), g is not a primitive root. */
            if (mod_pow64(g, phi / q, p) == 1)
                return false;
        }
    }

    if (m > 1) {
        if (mod_pow64(g, phi / m, p) == 1)
            return false;
    }
    return true;                          /* g is primitive root modulo p */
}

/* ------ Main function a(idx_t n) ------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but avoid UB */

    init_sequence_cache();
    init_prime_table();

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < (size_t)n) {
        /* Find next prime number. */
        uint64_t p = 0;
        while (true) {
            if (is_prime_simple(next_candidate)) {
                p = next_candidate;
                add_prime(p);               /* keep prime list in sync */
                next_candidate += 2;
                break;
            }
            next_candidate += 2;            /* skip even numbers */
        }

        if (p <= 3) continue;                /* 2 and 3 are not in the sequence */

        if (check_prime(p)) {
            add_term((seq_t)p);
        }
    }

    return terms[n - 1];
}
