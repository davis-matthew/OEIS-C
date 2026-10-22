/*
 * A383612  Numbers k such that 2 + val(k!, 2) < p + val(k!, p), where p is
 *          the largest prime <= k and val(r, m) is the valuation of r at m.
 * https://oeis.org/A383612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   legendre_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence definition:
   k is a term if 2 + val(k!,2) < p + val(k!,p),
   where p = largest prime ≤ k and val(r,m) is the exponent of m in r!.
   a(1) = 3, a(2) = 5, a(3) = 7, … */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/legendre_val.h"

/* static cache for terms */
static seq_t *terms = NULL;
static size_t term_cnt = 0;
static size_t term_cap = 0;

/* static cache for primes */
static uint8_t *is_prime = NULL;
static uint32_t *prev_prime = NULL;
static size_t prime_limit = 0;
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* ensure primality data up to 'limit' (inclusive) */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    size_t new_limit = (size_t)limit;
    /* resize prime flags and previous‑prime arrays */
    uint8_t  *new_is_prime  = (uint8_t  *)realloc(is_prime,
                                               (new_limit + 1) * sizeof(uint8_t));
    uint32_t *new_prev_prime = (uint32_t*)realloc(prev_prime,
                                               (new_limit + 1) * sizeof(uint32_t));
    if (!new_is_prime || !new_prev_prime) exit(EXIT_FAILURE);
    is_prime  = new_is_prime;
    prev_prime = new_prev_prime;

    /* initialise all entries as potential primes (except 0,1) */
    for (size_t i = 0; i <= new_limit; ++i) {
        if (i < 2) is_prime[i] = 0;
        else      is_prime[i] = 1;
    }

    /* simple sieve of Eratosthenes */
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* rebuild prime list and prev_prime array */
    prime_cnt = 0;
    uint32_t last_prime = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *new_list = (uint32_t*)realloc(prime_list,
                                              new_cap * sizeof(uint32_t));
                if (!new_list) exit(EXIT_FAILURE);
                prime_list = new_list;
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = (uint32_t)i;
            last_prime = (uint32_t)i;
        }
        prev_prime[i] = last_prime;
    }
    prev_prime[0] = prev_prime[1] = 0;

    prime_limit = new_limit;
}

/* add a term to the cache */
static void add_term(idx_t k)
{
    if (term_cnt == term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 16;
        seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_terms) exit(EXIT_FAILURE);
        terms = new_terms;
        term_cap = new_cap;
    }
    terms[term_cnt++] = (seq_t)k;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* safety: offset = 1 */

    static idx_t last_k = 0;  /* largest k processed so far */

    while (term_cnt < (size_t)n) {
        ++last_k;
        ensure_primes(last_k);

        uint32_t p = prev_prime[(size_t)last_k];
        if (p == 0) continue;          /* k < 2, no prime <= k */

        uint64_t val2 = legendre_val(last_k, 2u);
        uint64_t valp = legendre_val(last_k, p);

        if ( (uint64_t)2 + val2 < (uint64_t)p + valp )
            add_term(last_k);
    }
    return terms[(size_t)(n - 1)];
}
