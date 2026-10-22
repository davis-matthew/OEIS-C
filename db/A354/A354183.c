/*
 * A354183  Primes p such that p divides 2^((p-1)/x) - 1, where x is the
 *          greatest prime factor of p - 1.
 * https://oeis.org/A354183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   gpf, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354183
   a(n) = the n-th prime p such that p divides 2^((p-1)/x) – 1,
   where x is the greatest prime factor of (p-1).  */
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gpf.h"
#include "../utils/mod_pow.h"

/* Cached sequence values */
static seq_t *terms = NULL;      /* dynamically grown array of terms */
static size_t   terms_sz = 0;    /* number of stored terms   */
static size_t   terms_cap = 0;   /* current capacity         */

/* Search state */
static uint64_t next_candidate = 3;   /* candidate prime to test next */
static bool     initialized = false;  /* whether the module is initialized */

/* Ensure capacity of terms array */
static void grow_terms(void)
{
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    } else if (terms_sz >= terms_cap) {
        terms_cap <<= 1;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
}

/* Append a new term to the cached sequence */
static void append_term(seq_t v)
{
    grow_terms();
    terms[terms_sz++] = v;
}

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms computed */
    if (!initialized) {
        initialized = true;
    }

    while (terms_sz < (size_t)n) {
        /* Find the next prime candidate */
        while (!is_prime((idx_t)next_candidate)) {
            next_candidate += 2;
        }

        /* Compute largest prime factor x of (p-1) */
        uint64_t p = next_candidate;
        uint64_t x = gpf(p - 1);
        uint64_t m = (p - 1) / x;

        /* Check if p | 2^m - 1 using modular exponentiation */
        seq_t mod = (seq_t)p;
        seq_t r   = mod_pow((seq_t)2, m, mod);

        if (r == (seq_t)1) {
            append_term(mod);
        }

        /* Advance to the next odd number for the next iteration */
        next_candidate += 2;
    }

    return terms[n - 1];
}
