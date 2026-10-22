/*
 * A342839  Numbers k such that there are more primes in the interval [4*k+1,
 *          5*k] than there are in the interval [3*k+1, 4*k].
 * https://oeis.org/A342839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the nth term of OEIS sequence A342839.
   Definition: a(n) is the n-th positive integer k such that
   #primes in [4*k+1, 5*k] > #primes in [3*k+1, 4*k].
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIEVE_LIMIT 8000000UL   /* 5*k <= SIEVE_LIMIT guarantees all k needed */

static bool init_done = false;
static unsigned char *is_prime = NULL;      /* 1 = prime, 0 = composite */
static uint32_t *pi = NULL;                 /* prefix counts of primes */
static idx_t *terms = NULL;                 /* list of k values satisfying the inequality */
static size_t terms_size = 0;               /* how many terms have been stored */
static size_t terms_cap   = 0;               /* current capacity of the terms array */

/* Grow the terms array when necessary */
static void grow_terms(void)
{
    size_t new_cap = terms_cap == 0 ? 1024 : terms_cap * 2;
    idx_t *new_ptr = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
    if (!new_ptr) exit(EXIT_FAILURE);
    terms = new_ptr;
    terms_cap = new_cap;
}

/* Build a sieve of Eratosthenes up to SIEVE_LIMIT and the prefix prime counts */
static void build_sieve(void)
{
    size_t limit = SIEVE_LIMIT;
    is_prime = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    pi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!pi) exit(EXIT_FAILURE);
    pi[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        pi[i] = pi[i - 1] + is_prime[i];
}

/* Pre-compute all k up to the sieve limit that satisfy the condition */
static void init_sequence(void)
{
    build_sieve();

    for (idx_t k = 1; 5 * (uint64_t)k <= SIEVE_LIMIT; ++k) {
        uint32_t c1 = pi[5 * (uint64_t)k] - pi[4 * (uint64_t)k];
        uint32_t c2 = pi[4 * (uint64_t)k] - pi[3 * (uint64_t)k];
        if (c1 > c2) {
            if (terms_size == terms_cap) grow_terms();
            terms[terms_size++] = k;
        }
    }
    init_done = true;
}

/* Return the nth term of the sequence (n >= 1). 0 is returned for out of range. */
seq_t a(idx_t n)
{
    if (!init_done) init_sequence();
    if (n == 0 || n > terms_size) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
