/*
 * A359297  Primes prime(k) such that ( 8*(prime(k-1) - prime(k-2)) ) |
 *          (prime(k)^2 - 1).
 * https://oeis.org/A359297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p_k that satisfies 8·(p_{k-1} – p_{k-2})
 *            divides (p_k² – 1)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offsetting: a(1) = 5 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"   /* provides is_prime(idx_t) */

/* Prime storage */
static uint64_t *prime_list  = NULL;
static size_t   prime_count  = 0;
static size_t   prime_cap    = 0;

/* Term storage */
static seq_t *term_list  = NULL;
static size_t term_count = 0;
static size_t term_cap   = 0;

/* Next integer to test for primality (start at 2) */
static uint64_t next_candidate = 2;

/* Append a prime to the list, growing the array if needed. */
static void add_prime(uint64_t p)
{
    if (prime_count == prime_cap) {
        size_t new_cap = (prime_cap == 0 ? 4 : prime_cap * 2);
        prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_cap = new_cap;
    }
    prime_list[prime_count++] = p;
}

/* Append a term to the terms list, growing the array if needed. */
static void add_term(seq_t val)
{
    if (term_count == term_cap) {
        size_t new_cap = (term_cap == 0 ? 4 : term_cap * 2);
        term_list = (seq_t *)realloc(term_list, new_cap * sizeof(seq_t));
        term_cap = new_cap;
    }
    term_list[term_count++] = val;
}

/* Ensure that at least n terms are available. */
static void ensure_terms(idx_t n)
{
    while (term_count < (size_t)n) {
        uint64_t p = next_candidate;

        /* Advance to next candidate (skip even numbers after 2). */
        next_candidate = (p == 2 ? 3 : p + 2);

        if (is_prime(p)) {
            add_prime(p);

            /* We can test the property only after we have the first three primes. */
            if (prime_count >= 3) {
                uint64_t p_curr   = p;
                uint64_t p_prev1  = prime_list[prime_count - 2];
                uint64_t p_prev2  = prime_list[prime_count - 3];

                seq_t denom = (seq_t)8 * (seq_t)(p_prev1 - p_prev2);
                seq_t numer = (seq_t)p_curr * (seq_t)p_curr - (seq_t)1;

                if (denom != 0 && numer % denom == 0) {
                    add_term((seq_t)p_curr);
                }
            }
        }
    }
}

/* Public sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined before the offset */
    ensure_terms(n);
    return term_list[n - 1];
}
