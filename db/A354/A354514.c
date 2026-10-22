/*
 * A354514  Numbers k such that m - gpf(m) = k has solutions m >= 2, gpf =
 *          A006530.
 * https://oeis.org/A354514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 208, u16 n <= 8650, u32 n <= 8650, u64 n <= 8650,
 *            u128 n <= 8650, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th smallest non‑negative integer k such that
   there exists a prime factor p of k with gpf(k + p) = p.
   This implementation searches k in increasing order and tests
   the condition using trial division and the provided gpf() function. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gpf.h"

/* static state for incremental generation */
static seq_t *terms = NULL;          /* sorted list of terms */
static size_t terms_size = 0;        /* number of terms stored */
static size_t terms_cap = 0;         /* capacity of the terms array */
static uint64_t checked_max = 0;     /* maximum k that has been processed */

/* primes needed for factorization */
static uint64_t *prime_list = NULL;  /* array of primes */
static size_t prime_count = 0;       /* number of primes in the array */
static uint64_t last_prime_checked = 1;   /* last integer tested for primality */

/* Append a value to the terms array, growing it if necessary. */
static void grow_terms(void)
{
    if (terms_size >= terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);               /* bail out on allocation failure */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_size++] = (seq_t)checked_max;   /* store the term corresponding to checked_max */
}

/* Ensure prime_list contains all primes up to 'limit'.
   Uses trial division with the primes already found. */
static void ensure_primes_upto(uint64_t limit)
{
    while (last_prime_checked < limit) {
        ++last_prime_checked;
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > last_prime_checked) break;
            if (last_prime_checked % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            uint64_t *tmp = (uint64_t *)realloc(prime_list, (prime_count + 1) * sizeof(uint64_t));
            if (!tmp) exit(1);
            prime_list = tmp;
            prime_list[prime_count++] = last_prime_checked;
        }
    }
}

/* Test whether a given k satisfies the sequence condition. */
static bool qualifies(uint64_t k)
{
    if (k == 0) return true;            /* 0 is always a term (k = p - gpf(p)) */

    uint64_t temp = k;
    /* factor k using the primes collected so far */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            /* p is a prime factor of k */
            if (gpf(k + p) == p) return true;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {                     /* remaining factor is prime */
        if (gpf(k + temp) == temp) return true;
    }
    return false;
}

/* Ensure that at least 'n' terms have been generated. */
static void ensure_terms(idx_t n)
{
    if (n <= terms_size) return;                /* already have enough terms */

    /* The first term is 0; initialise structures if this is the first call. */
    if (terms_size == 0) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(1);
        terms_size = 0;
        checked_max = 0;
        /* 0 is a term */
        grow_terms();
    }

    /* Extend the search range until we have the required number of terms. */
    while (terms_size < (size_t)n) {
        ++checked_max;                          /* next k to test */
        ensure_primes_upto(checked_max);        /* make sure we have primes up to checked_max */
        if (qualifies(checked_max))
            grow_terms();                       /* add the term to the list */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined, but return 0 for safety */
    ensure_terms(n);
    return terms[n - 1];
}
